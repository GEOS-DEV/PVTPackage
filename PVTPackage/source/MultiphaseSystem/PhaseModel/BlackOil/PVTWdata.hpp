#pragma once
#include <vector>

namespace PVTPackage
{

	struct PVTWdata
	{
		friend class BlackOil_WaterModel;

		//--Data
		double ReferencePressure;
		double Bw;
		double Compressibility;
		double Viscosity;

	};
}
