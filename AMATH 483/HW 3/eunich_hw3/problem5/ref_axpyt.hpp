// ref_axpyt.hpp
#ifndef REF_AXPYT_HPP
#define REF_AXPYT_HPP

#include <vector>

// y ← a·x + y
template<typename T>
void axpy(T alpha, const std::vector<T>& x, std::vector<T>& y);

#endif // REF_AXPYT_HPP
