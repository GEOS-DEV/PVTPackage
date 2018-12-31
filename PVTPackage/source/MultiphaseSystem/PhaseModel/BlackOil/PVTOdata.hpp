#pragma once
#include <vector>

namespace PVTPackage
{

	struct PVTOdata
	{
		friend class BlackOil_OilModel;

		//--Data
		//
		std::vector<double> Rs;
		std::vector<double> BubblePressure;
		//Saturated
		std::size_t NSaturatedPoints;
		std::vector<double> SaturatedBo;
		std::vector<double> SaturatedViscosity;
		//Unsaturated
		std::vector<std::vector<double>> UndersaturatedPressure;   // Pressure - Pbub -> always start at 0
		std::vector<std::vector<double>> UndersaturatedBo;
		std::vector<std::vector<double>> UndersaturatedViscosity;

	private:
		//Pressure
		double MaxRelativePressure;
		double MinRelativePressure;

	};
}
