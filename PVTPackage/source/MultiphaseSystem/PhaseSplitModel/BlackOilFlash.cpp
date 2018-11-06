#include "BlackOilFlash.hpp"
#include <algorithm>

namespace PVTPackage
{
	BlackOilFlash::BlackOilFlash(): m_OilModel(nullptr), m_GasModel(nullptr),
	                                                                 m_WaterModel(nullptr)
	{
		
	}

	void BlackOilFlash::ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables)
	{
		//Compute Equilibrium
		ComputeEquilibrium(out_variables);

		//Finite difference derivatives
		ComputeFiniteDifferenceDerivatives(out_variables);

	}
}


