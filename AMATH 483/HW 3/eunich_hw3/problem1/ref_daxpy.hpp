#ifndef REF_DAXPY_HPP
#define REF_DAXPY_HPP

#include <vector>

// Compute y[i] = α * x[i] + y[i] for i = 0…n–1
void daxpy(double alpha,
           const std::vector<double> &x,
           std::vector<double> &y);

#endif // REF_DAXPY_HPP
