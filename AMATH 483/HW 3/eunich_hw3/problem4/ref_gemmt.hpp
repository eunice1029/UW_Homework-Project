// ref_gemmt.hpp
#ifndef REF_GEMMT_HPP
#define REF_GEMMT_HPP

#include <vector>

// C ← a·A·B + b·C
template<typename T>
void gemm(T alpha,
          const std::vector<std::vector<T>>& A,
          const std::vector<std::vector<T>>& B,
          T beta,
          std::vector<std::vector<T>>& C);

#endif // REF_GEMMT_HPP
