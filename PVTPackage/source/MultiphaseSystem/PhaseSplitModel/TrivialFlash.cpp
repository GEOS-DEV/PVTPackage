#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include <vector>
#include <numeric>

namespace PVTPackage
{
	void TrivialFlash::ComputeEquilibrium(double pressure, double temperature, std::vector<double> feed , PhaseSplitModelOutputVariables& out_variables)
	{

		const auto nbc = m_ComponentsProperties->NComponents;

		auto& gas_comp = out_variables.MoleComposition[PHASE_TYPE::GAS] = std::vector<double>(nbc);
		auto& oil_comp = out_variables.MoleComposition[PHASE_TYPE::OIL] = std::vector<double>(nbc);
		auto& water_comp = out_variables.MoleComposition[PHASE_TYPE::LIQUID_WATER_RICH] = std::vector<double>(nbc);

		auto KGasOil = ComputeWilsonGasOilKvalue(pressure, temperature);
		auto KWaterGas = ComputeWaterGasKvalue(pressure, temperature);
		auto KWaterOil = ComputeWaterOilKvalue(pressure, temperature);

		//Trivial split
		auto& Vo = out_variables.MoleFraction[PHASE_TYPE::OIL] = 0;
		auto& Vg = out_variables.MoleFraction[PHASE_TYPE::GAS] = 0;
		auto& Vw = out_variables.MoleFraction[PHASE_TYPE::LIQUID_WATER_RICH] = 0;

		for (size_t i = 0; i != nbc; ++i)
		{
			if (i != m_ComponentsProperties->WaterIndex)
			{
				//gas-oil
				if (KGasOil[i] > 1)
				{
					gas_comp[i] = feed[i];
					Vg = Vg + feed[i];
				}
				else
				{
					oil_comp[i] = feed[i];
					Vo = Vo + feed[i];
				}
			}
			else
			{
				//water-gas
				if (KWaterGas[i] < 1)
				{
					water_comp[i] = feed[i];
					Vw = Vw + feed[i];
				}
				else
				{
					gas_comp[i] = feed[i];
					Vg = Vg + feed[i];
				}
			}
		}

		
		//gas_comp = gas_comp / Vg;
		for (size_t i = 0; i != nbc; ++i)
		{
			gas_comp[i] = gas_comp[i] / Vg;
			oil_comp[i] = oil_comp[i] / Vo;
			water_comp[i] = water_comp[i] / Vw;
		}

		//Compute Phase State
		out_variables.set_PhaseState();


	}
}


