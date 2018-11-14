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

		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;
		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;

	protected:


	};

}


