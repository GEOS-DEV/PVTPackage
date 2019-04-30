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
