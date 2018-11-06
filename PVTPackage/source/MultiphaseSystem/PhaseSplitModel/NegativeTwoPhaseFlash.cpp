#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <vector>
#include <numeric>
#include "Utils/math.hpp"

namespace PVTPackage
{
	void NegativeTwoPhaseFlash::ComputeEquilibrium(MultiphaseSystemProperties& out_variables)
	{

		const auto& pressure = out_variables.Pressure;
		const auto& temperature = out_variables.Temperature;
		const auto& feed = out_variables.Feed;

		ASSERT(std::fabs(math::sum_array(feed) - 1.0) < 1e-12, "Feed sum must be 1");

		const auto nbc = m_ComponentsProperties.NComponents;

		auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value = std::vector<double>(nbc, 0);
		auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value = std::vector<double>(nbc, 0);

		auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
		auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;

		auto KGasOil = ComputeWilsonGasOilKvalue(pressure, temperature);

		//Check for machine-zero feed values
		const double epsilon = std::numeric_limits<double>::epsilon();
		std::list<size_t> positive_components;
		for (size_t i = 0; i != nbc; ++i)
		{
			if (feed[i] > epsilon)
			{
				positive_components.push_back(i);
			}
		}

		//Solve RachfordRice Equation
		double vapor_fraction = 0;
		vapor_fraction = SolveRachfordRiceEquation(KGasOil, feed, positive_components);
		gas_fraction = vapor_fraction;
		oil_fraction = 1.0 - gas_fraction;

		//Phase Mole Fractions
		gas_comp = std::vector<double>(nbc, 0);
		oil_comp = std::vector<double>(nbc, 0);
		for (auto it= positive_components.begin();it!= positive_components.end();++it)
		{
			gas_comp[*it] = feed[*it] / (1.0 + vapor_fraction * (KGasOil[*it] - 1.0));
			oil_comp[*it] = KGasOil[*it] * gas_comp[*it];
		}
		gas_comp = math::Normalize(gas_comp);
		oil_comp = math::Normalize(oil_comp);

		//Retrieve physical bounds from negative flash values
		if (gas_fraction>=1.0)
		{
			gas_fraction = 1.0;
			gas_comp = feed;
			oil_fraction = 0.0;
		}
		if (gas_fraction <= 0.0)
		{
			gas_fraction = 0.0;
			oil_fraction = 1.0; 
			oil_comp = feed;
		}

		//Phase Properties
		for (auto& m_PhaseModel : out_variables.PhaseModels)
		{
			auto& comp = out_variables.PhasesProperties.at(m_PhaseModel.first).MoleComposition.value;
			m_PhaseModel.second->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties[m_PhaseModel.first]);
		}

		//Compute Phase State
		set_PhaseState(out_variables);

	}
}


