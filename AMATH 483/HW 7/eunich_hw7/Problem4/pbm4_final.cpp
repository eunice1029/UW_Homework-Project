// pbm4_final.cpp

#include <iostream>
#include <vector>
#include <cuda_runtime.h>

// Macro for error checking after CUDA API calls
#define CHECK(call)                                                         \
    do {                                                                    \
        cudaError_t error = call;                                             \
        if (error != cudaSuccess) {                                           \
            std::cerr << "CUDA error at " << __FILE__ << ":" << __LINE__   \
                      << " code=" << error << " \""                          \
                      << cudaGetErrorString(error) << "\"" << std::endl;     \
            std::exit(EXIT_FAILURE);                                        \
        }                                                                   \
    } while (0)

int main() {

    // Create a list of memory sizes: 1 B, 2 B, 4 B, ..., 2 GiB
    const size_t MAX_BYTES = 2ULL * 1024 * 1024 * 1024;
    std::vector<size_t> sizes;
    for (size_t s = 1; s <= MAX_BYTES; s <<= 1) sizes.push_back(s);

    // Create CUDA events for timing (start and stop)
    cudaEvent_t start, stop;
    CHECK(cudaEventCreate(&start)); CHECK(cudaEventCreate(&stop));

    // Allocate pinned (page-locked) host memory and device memory
    char* h_buf = nullptr; char* d_buf = nullptr;
    CHECK(cudaMallocHost(&h_buf, MAX_BYTES)); CHECK(cudaMalloc(&d_buf, MAX_BYTES));

    // Print CSV header
    std::cout << "bytes,h2d_bw_Bps,d2h_bw_Bps\n";

    // Benchmark bandwidth for each memory size (Loop over each sizes)
    for (auto bytes : sizes) {
        float ms = 0.0f;

        // Measure (Host to Device) CPU to GPU transfer time and bandwidth
        CHECK(cudaEventRecord(start)); CHECK(cudaMemcpy(d_buf, h_buf, bytes, cudaMemcpyHostToDevice));
        CHECK(cudaEventRecord(stop)); CHECK(cudaEventSynchronize(stop));
        CHECK(cudaEventElapsedTime(&ms, start, stop)); // ms = time in milliseconds
        double H2D_bw = bytes / (ms * 1e-3); // Convert ms to seconds, bandwidth in Bytes/sec

        // Measure (Device to Host) GPU to CPU transfer time and bandwidth
        CHECK(cudaEventRecord(start)); CHECK(cudaMemcpy(h_buf, d_buf, bytes, cudaMemcpyDeviceToHost));
        CHECK(cudaEventRecord(stop)); CHECK(cudaEventSynchronize(stop));
        CHECK(cudaEventElapsedTime(&ms, start, stop));
        double D2H_bw = bytes / (ms * 1e-3);  // Bandwidth in Bytes/sec

        // Print result in CSV format
        std::cout << bytes << "," << H2D_bw << "," << D2H_bw << "\n";
    }

    // Cleanup all resources
    cudaFree(d_buf); cudaFreeHost(h_buf); cudaEventDestroy(start); cudaEventDestroy(stop);
    return 0;

}
