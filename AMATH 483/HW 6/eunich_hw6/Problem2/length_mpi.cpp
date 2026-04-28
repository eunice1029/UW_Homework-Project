// length_mpi.cpp
#include <mpi.h>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>

// integrand = sqrt(1 + (f'(x))^2) = (x^2 + 4)/(4x)
inline double integrand(double x) {
    return (x*x + 4.0) / (4.0 * x);
}

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, P;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    if (argc != 2) {
        if (rank == 0)
            std::cerr << "Usage: " << argv[0]
                      << " <num_intervals>\n";
        MPI_Finalize();
        return 1;
    }

    // parse inputs
    const std::size_t n = std::stoull(argv[1]);
    const double a = 1.0, b = 6.0;
    const double dx = (b - a) / static_cast<double>(n);

    // analytic exact length for error
    const double L_exact = 35.0/8.0 + std::log(6.0);

    // determine each rank's chunk
    std::size_t base = n / P;
    std::size_t start = rank * base;
    std::size_t end   = (rank == P-1 ? n : start + base);

    // synchronize and time the Riemann loop
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    double local_sum = 0.0;
    for (std::size_t i = start; i < end; ++i) {
        double x = a + (i + 1) * dx;  
        local_sum += integrand(x);
    }

    double t_local = MPI_Wtime() - t0;
    MPI_Barrier(MPI_COMM_WORLD);

    // reduce sums and max time to rank 0
    double global_sum = 0.0, t_max = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&t_local,   &t_max,      1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double L_num = global_sum * dx;
        double error = std::abs(L_num - L_exact);
        std::cout << std::fixed << std::setprecision(8)
                  << "n="      << n
                  << "  P="    << P
                  << "  L_num="<< L_num
                  << "  Error="<< error
                  << "  Time=" << t_max << "s\n";
    }

    MPI_Finalize();
    return 0;
}
