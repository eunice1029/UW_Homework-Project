// ref_axpyt.cpp
#include "ref_axpyt.hpp"
#include <stdexcept>

template<typename T>
void axpy(T alpha, const std::vector<T>& x, std::vector<T>& y) {
    if (x.empty() && y.empty()) 
        throw std::invalid_argument("Axpy: vector x and y are empty.");
    if (x.size() != y.size())
        throw std::invalid_argument("Axpy: x and y are not the same length. They must have the same length for calculation.");
    for (size_t i = 0; i < x.size(); ++i)
        y[i] += alpha * x[i];
}

// explicit instantiation
template void axpy<int>(int, const std::vector<int>&, std::vector<int>&);
template void axpy<float>(float, const std::vector<float>&, std::vector<float>&);
template void axpy<double>(double, const std::vector<double>&, std::vector<double>&);
