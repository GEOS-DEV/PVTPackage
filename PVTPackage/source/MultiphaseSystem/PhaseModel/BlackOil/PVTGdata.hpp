#pragma once
#include <vector>

namespace PVTPackage
{

	struct PVTGdata
	{
		friend class BlackOil_GasModel;

		//--Data
		//
		std::vector<double> Rv;
		std::vector<double> DewPressure;
		//Saturated
		size_t NSaturatedPoints;
		std::vector<double> SaturatedBg;
		std::vector<double> SaturatedViscosity;
		////Unsaturated 
		std::vector<std::vector<double>> UndersaturatedRv;   // always start at 0
		std::vector<std::vector<double>> UndersaturatedBg;
		std::vector<std::vector<double>> UndersaturatedViscosity;

	private:
		//Pressure
		double MaxRelativeRv;
		double MinRelativeRv;

	};
}
