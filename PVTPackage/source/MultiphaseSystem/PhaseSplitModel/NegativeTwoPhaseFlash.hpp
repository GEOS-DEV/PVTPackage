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

<<<<<<< HEAD
		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;
		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;
=======
		void set_PhaseState(MultiphaseSystemProperties& out_variables) override
		{

			out_variables.PhaseState = PhaseStateMap.at
			({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
				 out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0.,
				 false
			 });
		}

		bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) override;
>>>>>>> 489cf64f5560b9a2d2f0f4030fb7292341915c23

	protected:


	};

}


