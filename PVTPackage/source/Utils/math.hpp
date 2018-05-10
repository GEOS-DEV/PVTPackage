#pragma once
#ifndef MATH_HPP_INCLUDED
#define MATH_HPP_INCLUDED

// Std library includes
#include <vector>
#include <type_traits>
#include <numeric>

namespace math
{

template <typename Array>
typename Array::value_type sum_array(const Array& args)
{
  return std::accumulate(args.begin(), args.end(), Array::value_type(0), std::plus<typename Array::value_type>());
}

template <typename... Ts, typename std::common_type<int, Ts...>::type* = nullptr>
typename std::common_type<int, Ts...>::type sum(Ts... args)
{
  // With C++17 fold expressions we could just write:
  // return (0 + ... + args);
  using type = typename std::common_type<int, Ts...>::type;
  std::array<type, sizeof...(args)> temp{ args... };
  return sum_array(temp);
}

template <typename Array>
typename Array::value_type prod_array(const Array& args)
{
  return std::accumulate(args.begin(), args.end(), Array::value_type(1), std::multiplies<typename Array::value_type>());
}

template <typename... Ts, typename std::common_type<int, Ts...>::type* = nullptr>
typename std::common_type<int, Ts...>::type prod(Ts... args)
{
  // With C++17 fold expressions we could just write:
  // return (0 * ... * args);
  using type = typename std::common_type<int, Ts...>::type;
  std::array<type, sizeof...(args)> temp{ args... };
  return prod_array(temp);
}

template<typename T>
bool isNonNegative(const T& in)
{
  return in >= 0;
}

template<typename T>
bool isNonNegative(const std::vector<T>& in)
{
  bool res = true;
  for (std::size_t i = 0; i != in.size(); ++i)
    if (in[i] < std::numeric_limits<T>::epsilon()) return false;
  return res;
}

template<typename T>
bool isLowerThanOne(const std::vector<T>& in)
{
	bool res = true;
	for (std::size_t i = 0; i != in.size(); ++i)
		if (in[i] > 1 ) return false;
	return res;
}

template<typename T>
bool isPositive(const T& in)
{
  return in > std::numeric_limits<T>::epsilon();
}

template<typename T>
bool isPositive(const std::vector<T>& in)
{
  bool res = true;
  for (std::size_t i = 0; i != in.size(); i++)
    if (in[i] + std::numeric_limits<T>::epsilon() <= 0) return false;
  return res;
}


template<typename T>
bool isNotEqual(const T& in, T val)
{
  return in != val;
}

template<typename T>
bool isNotEqual(const std::vector<T>& in, T val)
{
  bool res = true;
  for (std::size_t i = 0; i != in.size(); i++)
    if (in[i] == val) return false;
  return res;
}

template<typename T>
bool isRightSize(unsigned int n, const std::vector<std::vector<T>>& in)
{
  bool res = true;
  for (std::size_t i = 0; i != in.size(); i++)
    if (in[i].size() != n) return false;
  return res;
}

template<typename T>
bool isRightSize(unsigned int n, const std::vector<T>& in)
{
  return in.size() == n;
}

template<typename T>
void ExtendtoNDimension(std::vector<T>& in, unsigned int n)
{
  if (in.size() == 1)
    in.assign(n, in[0]);
}

template<typename T>
std::vector<T> ScalarMultiply(std::vector<T>& in, T scalar)
{
	std::vector<T> out(in.size());
	for (auto i = 0; i != in.size(); i++)
		out[i]= in[i]*scalar;
	return out;
}






}
#endif //MATH_HPP_INCLUDED
