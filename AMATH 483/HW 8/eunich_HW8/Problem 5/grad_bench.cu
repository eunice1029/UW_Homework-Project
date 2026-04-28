#include <cstdio>
#include <cmath>
#include <vector>
#include <complex>
#include <chrono>
#include <fftw3.h>
#include <cuda_runtime.h>
#include <cufft.h>


using HiResClock = std::chrono::high_resolution_clock;
constexpr double TWO_PI = 6.283185307179586476925286766559;


#define CUDA_CHECK(e) do { cudaError_t err = (e);                     \
    if (err) {                                                        \
        printf("CUDA error %s:%d  %s\n", __FILE__, __LINE__,          \
               cudaGetErrorString(err)); return 1; } } while (0)


__global__ void scale_k(cufftDoubleComplex* F, int n)
{
    int gx = blockIdx.x * blockDim.x + threadIdx.x;
    int gy = blockIdx.y * blockDim.y + threadIdx.y;
    int gz = blockIdx.z * blockDim.z + threadIdx.z;
    if (gx >= n || gy >= n || gz >= n) return;

    int kx = (gx <= n/2) ?  gx : gx - n;
    int ky = (gy <= n/2) ?  gy : gy - n;
    int kz = (gz <= n/2) ?  gz : gz - n;

    size_t idx = (size_t(gz) * n + gy) * n + gx;
    cufftDoubleComplex v = F[idx];

    // store derivative w.r.t. y   (just one component is enough for timing) 
    F[idx].x = -TWO_PI * ky * v.y;
    F[idx].y =  TWO_PI * ky * v.x;

    // use kx,kz once so the “unused” warnings disappear 
    if (kx + kz == 123456789) F[idx].x += 0.0;  
}



int main()
{
    const int n0 = 16, nmax = 256, ntrial = 3;
    FILE* csv = fopen("grad_bench.csv", "w");
    fprintf(csv, "n,CPU_sec,CPU_FLOPs,GPU_sec,GPU_FLOPs\n");

    for (int n = n0; n <= nmax; n *= 2)
    {
        size_t N = size_t(n) * n * n;
        double flop_grad = 20.0 * double(N) * std::log2(double(N));

        // CPU  (FFTW)  
        std::vector<std::complex<double>> in(N), spec(N), out(N);

        fftw_plan fwd = fftw_plan_dft_3d(n, n, n,
                reinterpret_cast<fftw_complex*>(in.data()),
                reinterpret_cast<fftw_complex*>(spec.data()),
                FFTW_FORWARD, FFTW_MEASURE);

        fftw_plan inv = fftw_plan_dft_3d(n, n, n,
                reinterpret_cast<fftw_complex*>(spec.data()),
                reinterpret_cast<fftw_complex*>(out.data()),
                FFTW_BACKWARD, FFTW_MEASURE);

        double cpu_sum = 0.0;
        for (int t = 0; t < ntrial; ++t) {
            auto t0 = HiResClock::now();

            fftw_execute(fwd);

            // multiply by i·k_y 
            for (size_t idx = 0; idx < N; ++idx) {
                int gy = (idx / n) % n;
                int ky = (gy <= n/2) ? gy : gy - n;
                spec[idx] *= std::complex<double>(0, TWO_PI * ky);
            }

            fftw_execute(inv);
            fftw_execute(inv);
            fftw_execute(inv);

            cpu_sum += std::chrono::duration<double>(HiResClock::now() - t0).count();
        }
        double cpu_avg  = cpu_sum / ntrial;
        double cpu_flop = flop_grad / cpu_avg;

        fftw_destroy_plan(fwd); fftw_destroy_plan(inv);

        // GPU (cuFFT) 
        cufftDoubleComplex *d_data, *d_spec;
        CUDA_CHECK(cudaMalloc(&d_data, N * sizeof(cufftDoubleComplex)));
        CUDA_CHECK(cudaMalloc(&d_spec, N * sizeof(cufftDoubleComplex)));

        cufftHandle planF, planI;
        cufftPlan3d(&planF, n, n, n, CUFFT_Z2Z);
        cufftPlan3d(&planI, n, n, n, CUFFT_Z2Z);

        dim3 B(8, 8, 8);
        dim3 G((n + B.x - 1) / B.x,
               (n + B.y - 1) / B.y,
               (n + B.z - 1) / B.z);

        float gpu_msec = 0.0f;
        cudaEvent_t e0, e1;  cudaEventCreate(&e0);  cudaEventCreate(&e1);

        for (int t = 0; t < ntrial; ++t) {
            cudaEventRecord(e0);

            cufftExecZ2Z(planF, d_data, d_spec, CUFFT_FORWARD);
            scale_k<<<G, B>>>(d_spec, n);
            cufftExecZ2Z(planI, d_spec, d_data, CUFFT_INVERSE);
            cufftExecZ2Z(planI, d_spec, d_data, CUFFT_INVERSE);
            cufftExecZ2Z(planI, d_spec, d_data, CUFFT_INVERSE);

            cudaEventRecord(e1);  cudaEventSynchronize(e1);
            float ms;  cudaEventElapsedTime(&ms, e0, e1);
            gpu_msec += ms;
        }
        double gpu_avg  = (gpu_msec / ntrial) * 1e-3;   // → seconds
        double gpu_flop = flop_grad / gpu_avg;

        cufftDestroy(planF); cufftDestroy(planI);
        cudaFree(d_data); cudaFree(d_spec);

        fprintf(csv, "%d,%.6e,%.6e,%.6e,%.6e\n",
                n, cpu_avg, cpu_flop, gpu_avg, gpu_flop);

        printf("n=%3d  CPU %.3fs  GPU %.3fs\n", n, cpu_avg, gpu_avg);
    }

    fclose(csv);
    return 0;
}
