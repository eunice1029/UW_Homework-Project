#include "ref_daxpy.hpp"
#include <chrono>
#include <vector>
#include <random>
#include <iostream>
#include <iomanip>
#include <stdexcept>

// Level‑1 BLAS: y ← α·x + y
void daxpy(double alpha,
           const std::vector<double>& x,
           std::vector<double>& y)
{

    if (x.empty() && y.empty())
        throw std::invalid_argument("Daxpy: x and y are both empty.");

    if (x.empty() || y.empty())
        throw std::invalid_argument("Daxpy: One of the vectors is empty while the other is not — size mismatch.");

    if (x.size() != y.size())
        throw std::invalid_argument("Daxpy: x and y are not the same length. They must have the same length for calculation.");
    for (size_t i = 0; i < x.size(); ++i) {
        y[i] += alpha * x[i];  // 1 multiply + 1 add = 2 FLOPs
    }
}

// int main() {
//     using Clock = std::chrono::high_resolution_clock;

//     const int n_min    = 2;
//     const int n_max    = 512;
//     const int n_trials = 3;    // repeats per vector‐length
//     const double alpha = 2.5;     // coefficient

//     std::mt19937_64 rng(42);
//     std::uniform_real_distribution<double> dist(0.0, 1.0);

//     // CSV header
//     std::cout << "#n,FLOP/s\n";

//     for (int n = n_min; n <= n_max; ++n) {
//         // initialize x and y
//         std::vector<double> x(n), y(n);
//         for (int i = 0; i < n; ++i) {
//             x[i] = dist(rng);
//             y[i] = dist(rng);
//         }

//         long double total_time = 0.0L;  // seconds

//         // time n_trials calls
//         for (int t = 0; t < n_trials; ++t) {
//             auto start = Clock::now();
//             daxpy(alpha, x, y);
//             auto stop  = Clock::now();
//             auto ns    = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
//             total_time += ns * 1e-9L;      // to seconds
//         }

//         long double avg_time = total_time / n_trials;
//         // each call does 2*n FLOPs
//         long double flops = (2.0L * n) / avg_time;

//         std::cout
//             << n << ", "
//             << std::fixed << std::setprecision(2)
//             << flops << "\n";
//     }

//     return 0;
// }
