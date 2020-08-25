/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp"
#include "Utils/math.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{

	bool FreeWaterFlash::ComputeEquilibrium(MultiphaseSystemProperties & out_variables)
	{

		// Equilibrium convergence parameters
		const int max_SSI_iterations = 100;
		const double fug_epsilon = 1e-8;

		const auto& pressure = out_variables.Pressure;
		const auto& temperature = out_variables.Temperature;
		const auto& feed = out_variables.Feed;

		ASSERT(std::fabs(math::sum_array(feed) - 1.0) < 1e-12, "Feed sum must be 1");

		//Water
		const auto& water_feed = feed[m_WaterIndex];

		const auto nbc = m_ComponentsProperties.NComponents;

		auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value;
		auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value;
		auto& water_comp = out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH).MoleComposition.value;

		auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
		auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;
		auto& water_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value;

		auto& oil_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).LnFugacityCoefficients.value;
		auto& gas_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).LnFugacityCoefficients.value;
		auto& water_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH).LnFugacityCoefficients.value;

		std::vector<double> fug_ratio(nbc), fug_ratio_w(nbc);
		
		//Compute Equilibrium ratios
		auto KGasLiquid = ComputeWilsonGasLiquidKvalue(pressure, temperature);
		KGasLiquid[m_WaterIndex] = std::numeric_limits<double>::max(); //std::numeric_limits<double>::infinity(); //No water in oil
		auto KWater_GasWater = ComputeWaterGasKvalue(pressure, temperature)[m_WaterIndex];
		double KWater_OilWater = 0.0;

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
		water_comp.assign(nbc, 0.0);


		bool three_phase = false;
		int total_nb_iter = 0;
		for (int iter = 0; iter < max_SSI_iterations; ++iter)
		{

			//Test phase state - 3 or less
			three_phase = IsThreePhase(feed, KGasLiquid, positive_components, KWater_GasWater, KWater_OilWater, water_feed);

			double vapor_fraction = 0;
			if (three_phase)
			{
				// Solve Modifier Rachford-Rice Equation
				gas_fraction = SolveModifiedRachfordRiceEquation(KGasLiquid, feed, positive_components, KWater_GasWater, KWater_OilWater, water_feed);

				// Assign phase compositions

				double K_star_w = (1.0 - KWater_GasWater) / (1.0 - KWater_OilWater);
				double K_z_w = (1.0 - water_feed) / (1.0 - KWater_OilWater);
		
				for (auto ic : positive_components)
				{
					if (ic != m_WaterIndex)
					{
						const double K = (KGasLiquid[ic] - K_star_w);
						oil_comp[ic] = feed[ic] / (K_z_w + gas_fraction * K);
						gas_comp[ic] = KGasLiquid[ic] * oil_comp[ic];
					}
				}
				oil_comp[m_WaterIndex] = KWater_OilWater;
				gas_comp[m_WaterIndex] = KWater_GasWater;
			}
			else
			{
				
				// Solve Rachford-Rice Equation
				vapor_fraction = SolveRachfordRiceEquation(KGasLiquid, feed, positive_components);
				gas_fraction = vapor_fraction;
				oil_fraction = 1.0 - gas_fraction;
				water_fraction = 0.0;

				// Assign phase compositions
				for (auto ic : positive_components)
				{
					oil_comp[ic] = feed[ic] / (1.0 + vapor_fraction * (KGasLiquid[ic] - 1.0));
					gas_comp[ic] = KGasLiquid[ic] * oil_comp[ic];
				}

				//auto KWater_GasWater_max = water_feed / (gas_fraction*(1.0 - KWater_OilWater) + KWater_OilWater);
				
			}

			oil_comp = math::Normalize(oil_comp);
			gas_comp = math::Normalize(gas_comp);
			water_comp[m_WaterIndex] = 1.0;

			// Compute phase fugacity
			for (auto it = out_variables.PhaseModels.begin(); it != out_variables.PhaseModels.end(); ++it)
			{
				auto phase_type = (*it).first;
				auto eos_phase_model = std::dynamic_pointer_cast<CubicEoSPhaseModel>((*it).second);
				auto& comp = out_variables.PhasesProperties.at(phase_type).MoleComposition.value;
				eos_phase_model->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties.at(phase_type));
			}

			// Compute fugacity ratio and check convergence
			bool converged = true;

			for (auto ic : positive_components)
			{
				if (ic != m_WaterIndex)
				{
					fug_ratio[ic] = std::exp(oil_ln_fug[ic] - gas_ln_fug[ic]) * oil_comp[ic] / gas_comp[ic];
					if (std::fabs(fug_ratio[ic] - 1.0) > fug_epsilon)
						converged = false;
				}
				else
				{
					fug_ratio_w[ic] = std::exp(water_ln_fug[ic] - gas_ln_fug[ic]) * water_comp[ic] / gas_comp[ic];
				}
			}

			if (converged)
				break;

			// Update K-values
			for (auto ic : positive_components)
			{
				if (ic != m_WaterIndex)
				{
					KGasLiquid[ic] *= fug_ratio[ic];
				}
				else
				{
					KWater_GasWater *= fug_ratio_w[ic];   //HACK: Only this water k-value is updated
				}
			}

			total_nb_iter = iter;
		}

		// Retrieve physical bounds from negative flash values
		if (three_phase)
		{
			const auto max_gas_fraction = (1.0 - water_feed) / (1.0 - KWater_GasWater);

			if (gas_fraction <= 0.0 || gas_fraction >= max_gas_fraction)
			{
				PHASE_TYPE phase;
				if (gas_fraction >= max_gas_fraction)
				{
					gas_fraction = max_gas_fraction;
					water_fraction = (water_feed + gas_fraction + (KWater_OilWater - KWater_GasWater) - KWater_OilWater) / (1.0 - KWater_OilWater);
					for (auto ic : positive_components)
					{
						gas_comp[ic] = feed[ic] / gas_fraction;
					}
					gas_comp[m_WaterIndex] = (water_feed - water_fraction) / gas_fraction;
					phase = PHASE_TYPE::GAS;
				}
				else
				{
					gas_fraction = 0.0;
					oil_comp = feed;
					phase = PHASE_TYPE::OIL;
				}

				// Update phase properties since adjusting composition
				auto eos_phase = std::dynamic_pointer_cast<CubicEoSPhaseModel>(out_variables.PhaseModels.at(phase));
				auto & comp = out_variables.PhasesProperties.at(phase).MoleComposition.value;
				eos_phase->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties.at(phase));
			}

			water_fraction = (water_feed + gas_fraction + (KWater_OilWater - KWater_GasWater) - KWater_OilWater) / (1.0 - KWater_OilWater);

		}
		else
		{
			
			if (gas_fraction <= 0.0 || gas_fraction >= 1.0)
			{
				PHASE_TYPE phase;
				if (gas_fraction >= 1.0)
				{
					gas_fraction = 1.0;
					gas_comp = feed;
					phase = PHASE_TYPE::GAS;
				}
				else
				{
					gas_fraction = 0.0;
					oil_comp = feed;
					phase = PHASE_TYPE::OIL;
				}

				// Update phase properties since adjusting composition
				auto eos_phase = std::dynamic_pointer_cast<CubicEoSPhaseModel>(out_variables.PhaseModels.at(phase));
				auto & comp = out_variables.PhasesProperties.at(phase).MoleComposition.value;
				eos_phase->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties.at(phase));
			}

			water_fraction = 0.0;
		}

		oil_fraction = 1.0 - gas_fraction - water_fraction;

#ifndef NDEBUG
		//Compute global error - This should be ran on DEBUG MODE only
		auto total_error = 0.0;
		for (size_t i = 0; i != nbc; ++i)
		{
			total_error = total_error + feed[i] - (gas_comp[i] * gas_fraction + water_comp[i] * water_fraction + oil_comp[i] * oil_fraction);
		}
		ASSERT( std::abs(total_error) < epsilon, "Mass conservation problem in flash");
#endif


		// Compute final phase state
		set_PhaseState(out_variables);

		return total_nb_iter != max_SSI_iterations;


	}

	bool FreeWaterFlash::IsThreePhase(const std::vector<double>& feed, const std::vector<double>& Kvalues, const std::list<size_t>& non_zero_index,
		const double KWater_GasWater, const double KWater_OilWater, double water_feed)
	{
		double epsilon = std::numeric_limits<double>::epsilon();

		//Min and Max Kvalues for non-zero composition
		double max_K = 0, min_K = 1 / epsilon;
		for (auto it = non_zero_index.begin(); it != non_zero_index.end(); ++it)
		{
			if (static_cast<size_t>(std::distance(non_zero_index.begin(), it)) != m_WaterIndex)
			{
				if (Kvalues[*it] > max_K)
					max_K = Kvalues[*it];
				if (Kvalues[*it] < min_K)
					min_K = Kvalues[*it];
			}
		}

		const auto v_bar = -(water_feed - KWater_OilWater) / (KWater_OilWater - KWater_GasWater);
		const auto Klimit = 1.0 - (KWater_GasWater - KWater_OilWater) / (1.0 - KWater_OilWater);
		if (max_K <= Klimit)
		{
			return KWater_GasWater > KWater_OilWater;
		}

		if (min_K >= Klimit)
		{
			return KWater_GasWater <= KWater_OilWater;
		}
		
		const auto vmin = (-1.0 - (KWater_OilWater - water_feed) / (1.0 - KWater_OilWater)) / (-1.0 + max_K + (KWater_GasWater - KWater_OilWater) / (1.0 - KWater_OilWater));
		const auto vmax = (-1.0 - (KWater_OilWater - water_feed) / (1.0 - KWater_OilWater)) / (-1.0 + min_K + (KWater_GasWater - KWater_OilWater) / (1.0 - KWater_OilWater));

		if (v_bar> vmax)
		{
			return KWater_GasWater > KWater_OilWater;
		}

		if (v_bar <= vmin)
		{
			return KWater_GasWater <= KWater_OilWater;
		}


		return ModifiedRachfordRiceFunction(Kvalues, feed, non_zero_index, KWater_GasWater, KWater_OilWater, water_feed, v_bar)<0;

	}


	double FreeWaterFlash::ModifiedRachfordRiceFunction(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, 
		const double KWater_GasWater, const double KWater_OilWater, double water_feed, double x)
	{
		double val = 0;
		double K_star_w = (1.0 - KWater_GasWater) / (1.0 - KWater_OilWater);
		double K_z_w = (1.0 - water_feed) / (1.0 - KWater_OilWater);
		for (auto ic : non_zero_index)
		{
			if (ic != m_WaterIndex)
			{
				const double K = (Kvalues[ic] - K_star_w);
				val = val + feed[ic] * K / (K_z_w + x * K);
			}
		}
		return val;
	}

	double FreeWaterFlash::dModifiedRachfordRiceFunction_dx(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, 
		const double KWater_GasWater, const double KWater_OilWater, double water_feed, double x)
	{
		double val = 0;
		double K_star_w = (1.0 - KWater_GasWater) / (1.0 - KWater_OilWater);
		double K_z_w = (1.0 - water_feed) / (1.0 - KWater_OilWater);
		for (auto ic : non_zero_index)
		{
			if (ic != m_WaterIndex)
			{
				const double K = (Kvalues[ic] - K_star_w);
				const double R = K / (K_z_w + x * K);
				val = val - feed[ic] * R * R;
			}
		}
		return val;
	}

	double FreeWaterFlash::SolveModifiedRachfordRiceEquation(const std::vector<double>& Kvalues, const std::vector<double>& feed,
																const std::list<size_t>& non_zero_index, double KWater_GasWater,
																double KWater_OilWater, double water_feed)
	{
		double gas_phase_mole_fraction = 0;

		//Numerical Parameters //TODO: move them outside the function
		double SSI_tolerance = 1e-8;
		int max_SSI_iterations = 200;
		double Newton_tolerance = 1e-12;
		int max_Newton_iterations = 30;
		double epsilon = std::numeric_limits<double>::epsilon();

		//Min and Max Kvalues for non-zero composition
		double max_K = 0, min_K = 1 / epsilon;
		for (auto it = non_zero_index.begin(); it != non_zero_index.end(); ++it)
		{
			if (static_cast<size_t>(std::distance(non_zero_index.begin(), it)) != m_WaterIndex)
			{
				if (Kvalues[*it] > max_K)
					max_K = Kvalues[*it];
				if (Kvalues[*it] < min_K)
					min_K = Kvalues[*it];
			}
		}

		//Check for trivial solutions. This corresponds to bad Kvalues //TODO:to be fixed
		if (max_K < (1.0-(KWater_GasWater- KWater_OilWater)/(1.0 - KWater_OilWater)))
		{
			return gas_phase_mole_fraction = 0.0;
		}
		if (min_K > (1.0 - (KWater_GasWater - KWater_OilWater) / (1.0 - KWater_OilWater)))
		{
			return gas_phase_mole_fraction = 1.0;
		}

		//Solver
		//Find solution window
		double x_min = (-1.0 - (KWater_OilWater - water_feed) / (1.0 - KWater_OilWater)) / (-1.0 + max_K + (KWater_GasWater - KWater_OilWater) / (1.0 - KWater_OilWater));
		double x_max = (-1.0 - (KWater_OilWater - water_feed) / (1.0 - KWater_OilWater)) / (-1.0 + min_K + (KWater_GasWater - KWater_OilWater) / (1.0 - KWater_OilWater));
		double sqrt_epsilon = sqrt(epsilon);
		x_min = x_min + sqrt_epsilon * (std::fabs(x_min) + sqrt_epsilon);
		x_max = x_max - sqrt_epsilon * (std::fabs(x_max) + sqrt_epsilon);

		double current_error = 1 / epsilon;

		//SSI loop
		double func_x_min = 0, func_x_mid = 0, func_x_max = 0;
		int SSI_iteration = 0;
		while ((current_error > SSI_tolerance) && (SSI_iteration < max_SSI_iterations))
		{
			double x_mid = 0.5*(x_min + x_max);
			func_x_min = 0; func_x_mid = 0; func_x_max = 0;
			for (auto it = non_zero_index.begin(); it != non_zero_index.end(); ++it)
			{
				func_x_min = ModifiedRachfordRiceFunction(Kvalues, feed, non_zero_index, KWater_GasWater,  KWater_OilWater, water_feed, x_min);
				func_x_mid = ModifiedRachfordRiceFunction(Kvalues, feed, non_zero_index, KWater_GasWater, KWater_OilWater, water_feed, x_mid);
				func_x_max = ModifiedRachfordRiceFunction(Kvalues, feed, non_zero_index, KWater_GasWater, KWater_OilWater, water_feed, x_max);
			}

			ASSERT(!std::isnan(func_x_min), "Modified Rachford-Rice solver returns NaN");
			ASSERT(!std::isnan(func_x_mid), "Modified Rachford-Rice solver returns NaN");
			ASSERT(!std::isnan(func_x_max), "Modified Rachford-Rice solver returns NaN");

			if ((func_x_min < 0) && (func_x_max < 0))
			{
				return gas_phase_mole_fraction = 0.0;
			}

			if ((func_x_min > 1) && (func_x_max > 1))
			{
				return gas_phase_mole_fraction = 1.0;
			}

			if (func_x_min*func_x_mid < 0.0)
			{
				x_max = x_mid;
			}

			if (func_x_max*func_x_mid < 0.0)
			{
				x_min = x_mid;
			}

			current_error = std::min(std::fabs(func_x_max - func_x_min), std::fabs(x_max - x_min));

			SSI_iteration++;

			if (SSI_iteration == max_SSI_iterations)
				LOGWARNING("Rachford-Rice SSI reached max number of iterations");
		}
		gas_phase_mole_fraction = 0.5*(x_max + x_min);

		//Newton loop
		int Newton_iteration = 0;
		double Newton_value = gas_phase_mole_fraction;
		while ((current_error > Newton_tolerance) && (Newton_iteration < max_Newton_iterations))
		{
			double delta_Newton = -ModifiedRachfordRiceFunction(Kvalues, feed, non_zero_index, KWater_GasWater, KWater_OilWater, water_feed, Newton_value) / dModifiedRachfordRiceFunction_dx(Kvalues, feed, non_zero_index, KWater_GasWater, KWater_OilWater, water_feed, Newton_value);
			current_error = std::fabs(delta_Newton) / std::fabs(Newton_value);
			Newton_value = Newton_value + delta_Newton;
			Newton_iteration++;

			if (Newton_iteration == max_Newton_iterations)
				LOGWARNING("Rachford-Rice Newton reached max number of iterations");
		}
		return gas_phase_mole_fraction = Newton_value;

	}

}
