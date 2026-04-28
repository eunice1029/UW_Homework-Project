// ref_gemmt.cpp
#include "ref_gemmt.hpp"
#include <stdexcept>

template<typename T>
void gemm(T alpha,
          const std::vector<std::vector<T>>& A,
          const std::vector<std::vector<T>>& B,
          T beta,
          std::vector<std::vector<T>>& C)
{
    size_t m = A.size();
    if (m == 0) 
        throw std::invalid_argument("Gemm: A is empty, but it must be non-empty.");
    size_t p = A[0].size();

    if (p == 0)
        throw std::invalid_argument("Gemm: Matrix A has zero columns (invalid inner dimension).");

    for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != p)
            throw std::invalid_argument("Gemm: Matrix A is jagged (inconsistent row sizes).");
    }

    if (B.size() != p) 
        throw std::invalid_argument("Gemm: The inner dimensions of A and B do not match. B must have the same number of rows as A has columns.");
    size_t n = B[0].size();
    for (size_t i = 0; i < p; ++i) {
        if (B[i].size() != n)
            throw std::invalid_argument("Gemm: Matrix B has inconsistent row sizes (jagged matrix).");
    }

    if (C.size() != m) 
        throw std::invalid_argument("Gemm: The row count of C does not match A. C must be m × n, where A is m × p.");

    for (size_t i = 0; i < m; ++i) {
        if (C[i].size() != n)
            throw std::invalid_argument("Gemm: Matrix C is jagged (inconsistent row sizes).");
    }
    for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != p || C[i].size() != n)
            throw std::invalid_argument("Gemm: There are inconsistent row sizes.");
        for (size_t j = 0; j < n; ++j) {
            T sum = T{};
            for (size_t k = 0; k < p; ++k)
                sum += A[i][k] * B[k][j];
            C[i][j] = alpha * sum + beta * C[i][j];
        }
    }
}

// explicit instantiation
template void gemm<int>(int,
   const std::vector<std::vector<int>>&,
   const std::vector<std::vector<int>>&,
   int,
   std::vector<std::vector<int>>&);

template void gemm<float>(float,
   const std::vector<std::vector<float>>&,
   const std::vector<std::vector<float>>&,
   float,
   std::vector<std::vector<float>>&);

template void gemm<double>(double,
   const std::vector<std::vector<double>>&,
   const std::vector<std::vector<double>>&,
   double,
   std::vector<std::vector<double>>&);

