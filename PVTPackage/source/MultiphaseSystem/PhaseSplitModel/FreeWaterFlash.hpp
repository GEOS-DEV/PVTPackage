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

#pragma once
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{

	class FreeWaterFlash final : public CompositionalFlash
	{
	public:

		FreeWaterFlash(const ComponentProperties& component_properties)
			: CompositionalFlash(component_properties)
		{
			m_WaterIndex = component_properties.WaterIndex;

		}

		~FreeWaterFlash() override = default;


		void set_PhaseState(MultiphaseSystemProperties& out_variables) override
		{

			out_variables.PhaseState = PhaseStateMap.at
			({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
			   out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0.,
			   out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value > 0.
				});
		}

		bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) override;

		bool IsThreePhase(const std::vector<double>& feed, const std::vector<double>& Kvalues, const std::list<size_t>& non_zero_index, 
									const double KWater_GasWater, const double KWater_OilWater, double water_feed);
		double ModifiedRachfordRiceFunction(const std::vector<double>& Kvalues, const std::vector<double>& feed,
		                            const std::list<size_t>& non_zero_index, double KWater_GasWater,
		                            double KWater_OilWater,
		                            double water_feed, double x);
		double dModifiedRachfordRiceFunction_dx(const std::vector<double>& Kvalues, const std::vector<double>& feed,
		                                const std::list<size_t>& non_zero_index, double KWater_GasWater,
		                                double KWater_OilWater,
		                                double water_feed, double x);
		double SolveModifiedRachfordRiceEquation(const std::vector<double>& Kvalues, const std::vector<double>& feed,
		                                         const std::list<size_t>& non_zero_index, double KWater_GasWater,
												 double KWater_OilWater,double water_feed);


	protected:

		size_t m_WaterIndex;

	};

}
