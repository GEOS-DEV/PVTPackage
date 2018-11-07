#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <vector>
#include <numeric>
#include "Utils/math.hpp"

namespace PVTPackage
{
	bool NegativeTwoPhaseFlash::ComputeEquilibrium(MultiphaseSystemProperties & out_variables)
	{
		// Equilibrium convergence parameters
		const int max_SSI_iterations = 100;
		const double fug_epsilon = 1e-8;

		const auto& pressure = out_variables.Pressure;
		const auto& temperature = out_variables.Temperature;
		const auto& feed = out_variables.Feed;

		ASSERT(std::fabs(math::sum_array(feed) - 1.0) < 1e-12, "Feed sum must be 1");

		const auto nbc = m_ComponentsProperties.NComponents;

		auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value;
		auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value;

		auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
		auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;

		auto& oil_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).LnFugacityCoefficients.value;
		auto& gas_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).LnFugacityCoefficients.value;

		std::vector<double> fug_ratio(nbc);

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

		gas_comp.assign(nbc, 0.0);
		oil_comp.assign(nbc, 0.0);

		int iter;
		for (iter = 0; iter < max_SSI_iterations; ++iter)
		{
			// Solve Rachford-Rice Equation
			const double vapor_fraction = SolveRachfordRiceEquation(KGasOil, feed, positive_components);
			gas_fraction = vapor_fraction;
			oil_fraction = 1.0 - gas_fraction;

			// Assign phase compositions
			for (auto ic : positive_components)
			{
				oil_comp[ic] = feed[ic] / (1.0 + vapor_fraction * (KGasOil[ic] - 1.0));
				gas_comp[ic] = KGasOil[ic] * oil_comp[ic];
			}
      oil_comp = math::Normalize(oil_comp);
      gas_comp = math::Normalize(gas_comp);

			// Compute phase fugacity
			// TODO remove redundant computations, only fugacity needed
			for (auto & m_PhaseModel : out_variables.PhaseModels)
			{
				auto & comp = out_variables.PhasesProperties.at(m_PhaseModel.first).MoleComposition.value;
				m_PhaseModel.second->ComputeAllProperties(pressure, temperature, comp,
																									out_variables.PhasesProperties[m_PhaseModel.first]);
			}

			// Compute fugacity ratio and check convergence
			bool converged = true;

			for (auto ic : positive_components)
			{
				fug_ratio[ic] = std::exp(oil_ln_fug[ic] - gas_ln_fug[ic]) * oil_comp[ic] / gas_comp[ic];
				if (std::fabs(fug_ratio[ic] - 1.0) > fug_epsilon)
					converged = false;
			}

			if (converged)
				break;

			// Update K-values
      for (auto ic : positive_components)
      {
        KGasOil[ic] *= fug_ratio[ic];
      }
		}

		// Retrieve physical bounds from negative flash values
		if (gas_fraction >= 1.0)
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

		// Compute final phase properties
		for (auto & m_PhaseModel : out_variables.PhaseModels)
		{
			auto & comp = out_variables.PhasesProperties.at(m_PhaseModel.first).MoleComposition.value;
			m_PhaseModel.second->ComputeAllProperties(pressure, temperature, comp,
																								out_variables.PhasesProperties[m_PhaseModel.first]);
		}

		// Compute final phase state
		set_PhaseState(out_variables);

		return iter != max_SSI_iterations;

	}
}


