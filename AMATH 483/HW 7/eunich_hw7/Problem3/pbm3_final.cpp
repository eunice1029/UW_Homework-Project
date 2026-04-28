// pbm3_final.cpp

#include <iostream>
#include <vector>
#include <chrono>
#include <cblas.h>      // OpenBLAS C interface
#include <cuda_runtime.h>       // CUDA runtime API
#include <cublas_v2.h>      // cuBLAS library

using namespace std;
using Clock = chrono::high_resolution_clock;

// Define error checking for CUDA and cuBLAS
inline void check(cudaError_t error, const char* msg) {
    if (error != cudaSuccess) {
        cerr << "CUDA error at " << msg << ": " << cudaGetErrorString(error) << endl;
        exit(1);
    }
}

inline void check(cublasStatus_t st, const char* msg) {
    if (st != CUBLAS_STATUS_SUCCESS) {
        cerr << "cuBLAS error at " << msg << endl;
        exit(1);
    }
}



int main() {
    // n = 2,4,8,...,16384 (powers of 2 from 2 to 16384)
    vector<int> sizes;
    for (int n = 2; n <= 16384; n *= 2)
        sizes.push_back(n);
    const int trials = 5;

    cout << "N,openblas_flops,cublas_flops\n";

    // Create a cuBLAS handle
    cublasHandle_t handle;
    check(cublasCreate(&handle), "cublasCreate");

    // Iterate over matrix sizes
    for (int n : sizes) {
        size_t N2 = size_t(n) * n;
        double flops = 2.0 * n * n * n;     // Floating-point operations for dgemm

        //  Allocate and initialize host matrices
        vector<double> A(N2, 1.0), B(N2, 1.0), C(N2, 0.0);

        // Benchmark OpenBLAS on CPU
        double t_cpu = 0;
        for (int t = 0; t < trials; ++t) {
            auto t0 = Clock::now();
            cblas_dgemm(
                CblasColMajor, CblasNoTrans, CblasNoTrans,
                n, n, n, 1.0, A.data(), n, B.data(), n, 0.0, C.data(), n);

            auto t1 = Clock::now();
            t_cpu += chrono::duration<double>(t1 - t0).count();
        }
        double flops_cpu = flops / (t_cpu / trials); // FLOPS for CPU

        // Allocate GPU memory
        double *dA, *dB, *dC;
        check(cudaMalloc(&dA, N2 * sizeof(double)), "cudaMalloc A");
        check(cudaMalloc(&dB, N2 * sizeof(double)), "cudaMalloc B");
        check(cudaMalloc(&dC, N2 * sizeof(double)), "cudaMalloc C");

        // Copy host data to device
        check(cudaMemcpy(dA, A.data(), N2 * sizeof(double), cudaMemcpyHostToDevice), "cudaMemcpy A");
        check(cudaMemcpy(dB, B.data(), N2 * sizeof(double), cudaMemcpyHostToDevice), "cudaMemcpy B");

        // Set up CUDA event timers for GPU timing
        cudaEvent_t start, stop;
        check(cudaEventCreate(&start), "eventCreate start"); 
        check(cudaEventCreate(&stop), "eventCreate stop");

        // Benchmark cuBLAS on GPU
        float t_gpu_ms = 0;
        double alpha = 1.0, beta = 0.0;
        for (int t = 0; t < trials; ++t) {
            check(cudaEventRecord(start), "eventRecord start");
            check(cublasDgemm(handle, CUBLAS_OP_N, CUBLAS_OP_N, n, n, n, &alpha, dA, n, dB, n, &beta,  dC, n), "cublasDgemm");
            check(cudaEventRecord(stop), "eventRecord stop");
            check(cudaEventSynchronize(stop), "eventSync");
            float ms = 0;
            check(cudaEventElapsedTime(&ms, start, stop), "eventElapsed");
            t_gpu_ms += ms;
        }

        double flops_gpu = flops / ((t_gpu_ms / 1000.0) / trials); // FLOPS for GPU

        // Cleanup device memory and events
        cudaFree(dA);
        cudaFree(dB);
        cudaFree(dC);
        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        // Print result for csv
        cout << n << "," << flops_cpu << "," << flops_gpu << "\n";
    }

    // Destroy cuBLAS handle
    cublasDestroy(handle);
    return 0;
}
