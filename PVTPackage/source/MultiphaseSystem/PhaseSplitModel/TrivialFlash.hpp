#pragma once
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"

namespace PVTPackage
{

	class TrivialFlash final : public CompositionalFlash
	{
	public:

		TrivialFlash(const ComponentProperties& component_properties)
			: CompositionalFlash(component_properties)
		{
		}

		void set_PhaseState(MultiphaseSystemProperties& out_variables) override
		{

			out_variables.PhaseState = PhaseStateMap.at
			({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
				out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0. ,
				out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value > 0.
				});
		}

		~TrivialFlash() override = default;

		bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) override;

	protected:


	};

}


