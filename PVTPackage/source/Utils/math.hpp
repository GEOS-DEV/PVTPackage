#pragma once
#ifndef MATH_HPP_INCLUDED
#define MATH_HPP_INCLUDED

// Std library includes
#include <vector>
#include <type_traits>
#include <numeric>
#include "Assert.hpp"

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

template<typename T>
void FindSurrondingIndex(std::vector<T>& x, T xval, size_t& iminus, size_t& iplus)
{
	//Search for interval
	iminus = 0; iplus = 0;
	for (size_t i = 0; i != x.size(); ++i)
	{
		if (x[i] == xval)
		{
			iminus = i;
			iplus = i;
			break;
		}

		if (x[i] >= xval)
		{
			iminus = i - 1;
			iplus = i;
			break;
		}

		if (i == x.size() - 1)
		{
			iminus = x.size() - 2;
			iplus = x.size() - 1;
		}
	}

}

template<typename T>
std::vector<T> Interpolation1 (std::vector<T>& xin, std::vector<T>& yin, std::vector<T> xout)
{
	//return yout

	ASSERT(xin.size() == yin.size(), "Size mistmatch");

	std::vector<T> yout(xout.size());

	for (size_t n = 0;n != xout.size(); ++n)
	{
		T x = xout[n];
		ASSERT(xin[0] <= x, "Input x out of range, cannot extrapolate");
		//LOGWARNING(xin[xin.size() - 1] >= x, "Input x out of range, extrapolation");

		//Search for interval
		size_t i_minus=0, i_plus=0;
		FindSurrondingIndex(xin, x, i_minus, i_plus);

		if (i_minus==i_plus)
		{
			yout[n] = yin[i_minus];
		}
		else
		{
			//yout[n] = yin[i_minus] * (1 - (x - xin[i_minus]) / (xin[i_plus] - xin[i_minus])) + yin[i_plus] * ((x - xin[i_minus]) / (xin[i_plus] - xin[i_minus]));
			yout[n] = yin[i_minus] * (xin[i_plus] - x) / (xin[i_plus] - xin[i_minus]) + yin[i_plus] * ((x - xin[i_minus]) / (xin[i_plus] - xin[i_minus]));
		}


	}

	return yout;

}



template<typename T>
T LinearInterpolation(T dminus, T dplus, T xminus, T xplus)
{
	T f = dminus / (dminus + dplus);
	return f*xminus + (1-f)*xplus;
}


template<typename T>
T LinearInterpolation(T x1, T y1, T x2, T y2, T x3)
{
	return LinearInterpolation(x3-x1, x2-x3, y1, y2);
}


template<typename T>
T LinearExtrapolation(T x1, T y1, T x2, T y2, T x3)
{
	return (y2 - y1) / (x2 - x1) * (x3-x2) + y2;
}


template<typename T>
T LogInterpolation(T dminus, T dplus, T xminus, T xplus)
{
	T f = log(dminus) / (log(dminus) + log(dplus));
	T lnx = f*xminus + (1 - f)*xplus;
	return exp(lnx);
}

template<typename T>
std::vector<T> linspace(T a, T b, size_t num)
{
	// create a vector of length num
	std::vector<T> v(num);

	// now assign the values to the vector
	for (size_t i = 0; i != num; i++)
	{
		v[i] = a + i * ((b - a) / (num-1));
	}
	return v;
}


}
#endif //MATH_HPP_INCLUDED
