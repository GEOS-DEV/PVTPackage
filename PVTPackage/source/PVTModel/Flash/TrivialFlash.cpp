#include "PVTModel/Flash/TrivialFlash.hpp"
#include "PVTModel/ComponentProperties.hpp"
#include <vector>
#include <numeric>

namespace PVTPackage
{
	void TrivialFlash::ComputeEquilibrium(const FlashInputVariables& in_variables, FlashOutputVariables& out_variables)
	{

		const auto nbc = m_ComponentsProperties->NComponents;
		const auto Temperature = in_variables.get_Temperature();
		const auto Pressure = in_variables.get_Pressure();

		auto& gas_comp = out_variables.get_Composition(PHASE_TYPE::GAS) = std::vector<double>(nbc);
		auto& oil_comp = out_variables.get_Composition(PHASE_TYPE::OIL) = std::vector<double>(nbc);
		auto& water_comp = out_variables.get_Composition(PHASE_TYPE::LIQUID_WATER_RICH) = std::vector<double>(nbc);

		auto KGasOil = ComputeWilsonGasOilKvalue(Pressure, Temperature);
		auto KWaterGas = ComputeWaterGasKvalue(Pressure, Temperature);
		auto KWaterOil = ComputeWaterOilKvalue(Pressure, Temperature);

		//Trivial split
		auto& Vo = out_variables.get_MoleFraction(PHASE_TYPE::OIL) = 0;
		auto& Vg = out_variables.get_MoleFraction(PHASE_TYPE::GAS) = 0;
		auto& Vw = out_variables.get_MoleFraction(PHASE_TYPE::LIQUID_WATER_RICH) = 0;
		const auto feed = in_variables.get_Feed();

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
		
		//Compute Properties

		const auto res_oil = m_PhaseModel[PHASE_TYPE::OIL]->ComputeMoleDensity(Pressure, Temperature, oil_comp);
		const auto res_water = m_PhaseModel[PHASE_TYPE::LIQUID_WATER_RICH]->ComputeMoleDensity(Pressure, Temperature, water_comp);
		const auto res_gas = m_PhaseModel[PHASE_TYPE::GAS]->ComputeMoleDensity(Pressure, Temperature, gas_comp);

		out_variables.get_MoleDensity(PHASE_TYPE::OIL) = res_oil.MoleDensity;
		out_variables.get_MoleDensity(PHASE_TYPE::LIQUID_WATER_RICH) =  res_water.MoleDensity;
		out_variables.get_MoleDensity(PHASE_TYPE::GAS) = res_gas.MoleDensity;

	}
}


