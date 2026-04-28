// ref_gemvt.cpp
#include "ref_gemvt.hpp"
#include <stdexcept>

template<typename T>
void gemv(T alpha,
          const std::vector<std::vector<T>>& A,
          const std::vector<T>& x,
          T beta,
          std::vector<T>& y)
{
    size_t m = A.size();
    if (m == 0) throw std::invalid_argument("Gemv: A is empty, but it must be non-empty.");
    size_t n = A[0].size();

    if (x.size() != n || y.size() != m)
        throw std::invalid_argument("Gemv: dimension mismatch and so calculation stops.");

    if (n == 0)
        throw std::invalid_argument("Gemv: A has zero columns — invalid matrix-vector operation.");

    if (m == 0 || n == 0) 
        throw std::invalid_argument("Gemv: All vectors are empty.");


    for (size_t i = 0; i < m; ++i) {
        if (A[i].size() != n)
            throw std::invalid_argument("Gemv: There are inconsistent row lengths in A.");
        T sum = T{};
        for (size_t j = 0; j < n; ++j)
            sum += A[i][j] * x[j];
        y[i] = alpha * sum + beta * y[i];
    }
}

// explicit instantiation
template void gemv<int>(int, const std::vector<std::vector<int>>&,
                        const std::vector<int>&, int,
                        std::vector<int>&);
template void gemv<float>(float, const std::vector<std::vector<float>>&,
                          const std::vector<float>&, float,
                          std::vector<float>&);
template void gemv<double>(double, const std::vector<std::vector<double>>&,
                           const std::vector<double>&, double,
                           std::vector<double>&);
