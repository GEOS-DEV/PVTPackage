#pragma once
#include <vector>

namespace PVTPackage
{
	template <typename T>
	struct ScalarPropertyAndDerivatives
	{
		ScalarPropertyAndDerivatives(std::size_t ncomponents) :value(0),dP(0),dT(0),dz(std::vector<double>(ncomponents, 0)) {}

		T value;
		T dP;
		T dT;
		std::vector<T> dz;
	};

	template <typename T>
	struct VectorPropertyAndDerivatives
	{
		VectorPropertyAndDerivatives(std::size_t dim, std::size_t ncomponents) :	value(std::vector<T>(dim, 0)),
																		dP(std::vector<T>(dim, 0)),
																		dT(std::vector<T>(dim, 0)),
																		dz(std::vector<std::vector<T>>(dim, std::vector<T>(ncomponents,0))){}

		std::vector<T> value;
		std::vector<T> dP;
		std::vector<T> dT;
		std::vector<std::vector<T>> dz;


	};


}



