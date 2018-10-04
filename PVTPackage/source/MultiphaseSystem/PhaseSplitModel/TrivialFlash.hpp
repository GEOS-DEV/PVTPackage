#pragma once
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

namespace PVTPackage
{

	class TrivialFlash final : public CompositionalFlash
	{
	public:

		TrivialFlash(const ComponentProperties& component_properties)
			: CompositionalFlash(component_properties)
		{
		}

		~TrivialFlash() override = default;

		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;

	protected:


	};

}


