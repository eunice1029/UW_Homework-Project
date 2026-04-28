// length_threads.cpp
#include <iostream>
#include <vector>
#include <thread>
#include <cmath>
#include <chrono>
#include <iomanip>

// The integrand: sqrt(1 + (f'(x))^2) simplifies to (x^2+4)/(4x)
inline double integrand(double x) {
    return (x*x + 4.0) / (4.0 * x);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <num_intervals> <num_threads>\n";
        return 1;
    }

    const std::size_t n    = std::stoull(argv[1]);  // total partitions
    const int T            = std::stoi(argv[2]);    // number of threads
    const double a = 1.0, b = 6.0;
    const double dx = (b - a) / static_cast<double>(n);

    // exact analytic length for error calculation
    const double L_exact = 35.0/8.0 + std::log(6.0);

    // container for each thread's partial sum
    std::vector<double> partial(T, 0.0);

    auto worker = [&](int tid){
        std::size_t chunk = n / T;
        std::size_t start = tid * chunk;
        std::size_t end   = (tid == T-1 ? n : start + chunk);
        double sum = 0.0;
        for (std::size_t i = start; i < end; ++i) {
            double x = a + (i + 1) * dx;
            sum += integrand(x);
        }
        partial[tid] = sum;
    };

    // launch threads and time the computation
    auto t_start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    threads.reserve(T);
    for (int t = 0; t < T; ++t)
        threads.emplace_back(worker, t);
    for (auto& th : threads) th.join();
    auto t_end = std::chrono::high_resolution_clock::now();

    // combine results
    double total = 0.0;
    for (double v : partial) total += v;
    total *= dx;  // multiply by Δx

    // compute error
    double error = std::abs(total - L_exact);

    // output: n, T, numeric length, error, and time
    std::chrono::duration<double> dur = t_end - t_start;
    std::cout << std::fixed << std::setprecision(8)
              << "n="      << n
              << "  T="    << T
              << "  L_num="<< total
              << "  Error="<< error
              << "  Time=" << dur.count() << "s\n";

    return 0;
}
