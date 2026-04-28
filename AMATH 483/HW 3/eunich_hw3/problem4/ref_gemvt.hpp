// ref_gemvt.hpp
#ifndef REF_GEMVT_HPP
#define REF_GEMVT_HPP

#include <vector>

// y ← a·A·x + b·y
template<typename T>
void gemv(T alpha,
          const std::vector<std::vector<T>>& A,
          const std::vector<T>& x,
          T beta,
          std::vector<T>& y);

#endif // REF_GEMVT_HPP
