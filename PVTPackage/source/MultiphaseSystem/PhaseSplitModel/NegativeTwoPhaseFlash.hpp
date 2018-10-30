#pragma once
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"

namespace PVTPackage
{

	class NegativeTwoPhaseFlash final : public CompositionalFlash
	{
	public:

		NegativeTwoPhaseFlash(const ComponentProperties& component_properties)
			: CompositionalFlash(component_properties)
		{
		}

		~NegativeTwoPhaseFlash() override = default;

		void set_PhaseState(MultiphaseSystemProperties& out_variables) override
		{

			out_variables.PhaseState = PhaseStateMap.at
			({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
				out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0.,0 
				});
		}

		bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) override;

	protected:


	};

}


