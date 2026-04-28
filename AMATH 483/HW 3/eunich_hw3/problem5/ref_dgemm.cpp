#include "ref_dgemm.hpp"
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <stdexcept>

// Level‑3 BLAS: C ← α·A·B + β·C
void dgemm(double alpha,
           const std::vector<std::vector<double>>& A,
           const std::vector<std::vector<double>>& B,
           double beta,
           std::vector<std::vector<double>>& C)
{
    size_t m = A.size(); // rows in A, C
    if (m == 0) {
        throw std::invalid_argument("Dgemm: Matrix A has no rows - must be non-empty. The operation cannot proceed.");
    }

    size_t p = A[0].size(); // cols in A, rows in B
    

    if (p == 0)
        throw std::invalid_argument("Dgemm: matrix A has zero columns — invalid inner dimension.");

    // Check jagged matrix A
    for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != p)
            throw std::invalid_argument("Dgemm: Matrix A has inconsistent row sizes (jagged).");
    }

    // Check jagged matrix B
    if (B.size() != p) {
        throw std::invalid_argument("Dgemm: B must have the same number of rows as A has columns (B.size() != p).");
    }
    size_t n = B[0].size(); // cols in B, C
    if (n == 0)
        throw std::invalid_argument("Dgemm: matrix B has zero columns — must have at least one column.");

    for (size_t i = 0; i < p; ++i) {
        if (B[i].size() != n)
            throw std::invalid_argument("Dgemm: Matrix B has inconsistent row sizes (jagged).");
    }

    // Check matrix C dimensions
    if (C.size() != m) {
        throw std::invalid_argument("Dgemm: Matrix C must have the same number of rows as A.");
    }
    for (size_t i = 0; i < m; ++i) {
        if (C[i].size() != n)
            throw std::invalid_argument("Dgemm: Matrix C has inconsistent row sizes or incorrect column count.");
    }

    // C ← β·C
    for (size_t i = 0; i < m; ++i)
        for (size_t j = 0; j < n; ++j)
            C[i][j] *= beta;

    // C[i][j] += α * (A[i]·B[:,j])
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double sum = 0.0;
            for (size_t k = 0; k < p; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] += alpha * sum;
        }
    }
}



// int main() {
//     using Clock = std::chrono::high_resolution_clock;

//     const int n_min    = 2;
//     const int n_max    = 512;
//     const int n_trials = 3;    // repetitions per size
//     const double alpha = 1.25;    // α
//     const double beta  = 0.75;    // β

//     std::mt19937_64 rng(42);
//     std::uniform_real_distribution<double> dist(0.0, 1.0);

//     // CSV header
//     std::cout << "#n,FLOP/s\n";

//     for (int n = n_min; n <= n_max; ++n) {
//         // initialize A, B, C₀
//         std::vector<std::vector<double> > A(n, std::vector<double>(n)),
//                                       B(n, std::vector<double>(n)),
//                                       C0(n, std::vector<double>(n));
//         for (int i = 0; i < n; ++i) {
//             for (int j = 0; j < n; ++j) {
//                 A[i][j] = dist(rng);
//                 B[i][j] = dist(rng);
//                 C0[i][j] = dist(rng);
//             }
//         }

//         long double total_time = 0.0L;  // seconds

//         for (int t = 0; t < n_trials; ++t) {
//             // reset C each trial
//             std::vector<std::vector<double> > C = C0;

//             auto start = Clock::now();
//             dgemm(alpha, A, B, beta, C);
//             auto stop  = Clock::now();
//             auto ns    = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
//             total_time += ns * 1e-9L;
//         }

//         long double avg_time = total_time / n_trials;
//         // Theoretical FLOP count per call: (2n−1 adds + n multiplies) per dot + 2 flops scaling/add
//         // → (2n−1 + 2) = 2n+1 flops per C[i][j], over n^2 elements → 2n^3 + n^2
//         long double flop_count = 2.0L * n * n * n + 1.0L * n * n;
//         long double flops = flop_count / avg_time;

//         std::cout
//             << n << ", "
//             << std::fixed << std::setprecision(2)
//             << flops << "\n";
//     }

//     return 0;
// }
