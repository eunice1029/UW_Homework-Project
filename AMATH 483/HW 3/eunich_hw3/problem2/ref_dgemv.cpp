#include "ref_dgemv.hpp"
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <stdexcept>

// Level‑2 BLAS: y ← α·A·x + β·y
void dgemv(double alpha,
           const std::vector<std::vector<double> >& A,
           const std::vector<double>& x,
           double beta,
           std::vector<double>& y)
{
    const size_t m = A.size();
    if (m == 0) 
        throw std::invalid_argument("Dgemv: A is empty — matrix must be non-empty.");

    const size_t n = A[0].size();
    if (n == 0)
        throw std::invalid_argument("Dgemv: A has zero columns — matrix must have at least one column.");

    for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != n)
            throw std::invalid_argument("Dgemv: A has inconsistent row sizes (jagged matrix).");

    if (x.empty() || y.empty())
        throw std::invalid_argument("Dgemv: x or y is empty — both must be non-empty vectors.");

    if (x.size() != n || y.size() != m)
        throw std::invalid_argument("Dgemv: dimension mismatch — x must match columns of A, y must match rows of A.");
    
    }
    // y ← β·y
    for (size_t i = 0; i < m; ++i) {
        y[i] *= beta;      // 1 FLOP per element
    }
    // y[i] += α * (A[i]·x)
    for (size_t i = 0; i < m; ++i) {
        double sum = 0.0;
        for (size_t j = 0; j < n; ++j) {
            sum += A[i][j] * x[j];  // 1 multiply + 1 add per j except first add is just an add
        }
        // dot product does (n multiplies + (n−1) adds) = 2n−1 FLOPs
        y[i] += alpha * sum;        // 1 multiply + 1 add = 2 FLOPs
    }
}

// int main() {
//     using Clock = std::chrono::high_resolution_clock;

//     const int n_min    = 2;
//     const int n_max    = 512;
//     const int n_trials = 3;    // repetitions per n
//     const double alpha = 2.5;     // α
//     const double beta  = 1.5;     // β

//     std::mt19937_64 rng(42);
//     std::uniform_real_distribution<double> dist(0.0, 1.0);

//     // CSV header
//     std::cout << "#n,FLOP/s\n";

//     for (int n = n_min; n <= n_max; ++n) {
//         // initialize A (n×n), x (n), y₀ (n)
//         std::vector<std::vector<double> > A(n, std::vector<double>(n));
//         std::vector<double> x(n), y0(n);
//         for (int i = 0; i < n; ++i) {
//             x[i]  = dist(rng);
//             y0[i] = dist(rng);
//             for (int j = 0; j < n; ++j)
//                 A[i][j] = dist(rng);
//         }

//         long double total_time = 0.0L;  // seconds

//         // time n_trials runs
//         for (int t = 0; t < n_trials; ++t) {
//             // reset y = y0
//             std::vector<double> y = y0;

//             auto start = Clock::now();
//             dgemv(alpha, A, x, beta, y);
//             auto stop  = Clock::now();
//             auto ns    = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
//             total_time += ns * 1e-9L;      // to seconds
//         }

//         long double avg_time = total_time / n_trials;
//         // FLOP count per call = n·[(2n−1) + 2] = 2n^2 + n
//         long double flops = (2.0L * n * n + 1.0L * n) / avg_time;

//         std::cout
//             << n << ", "
//             << std::fixed << std::setprecision(2)
//             << flops << "\n";
//     }

//     return 0;
// }
