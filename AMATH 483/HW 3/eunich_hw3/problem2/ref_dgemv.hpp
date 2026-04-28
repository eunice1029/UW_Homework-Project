#ifndef REF_DGEMV_HPP
#define REF_DGEMV_HPP

#include <vector>

// y ← alpha*A*x + beta*y
void dgemv(double alpha,
           const std::vector<std::vector<double> > &A,
           const std::vector<double> &x,
           double beta,
           std::vector<double> &y);

#endif // REF_DGEMV_HPP
