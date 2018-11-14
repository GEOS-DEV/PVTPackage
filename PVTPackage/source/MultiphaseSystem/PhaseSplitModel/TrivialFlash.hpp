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

		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;

		~TrivialFlash() override = default;

		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;

	protected:


	};

}


