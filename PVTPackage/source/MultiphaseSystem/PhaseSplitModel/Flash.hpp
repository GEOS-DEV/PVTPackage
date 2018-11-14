#pragma once
#include "MultiphaseSystem/PVTEnums.hpp"

namespace PVTPackage
{
	struct MultiphaseSystemProperties;

	class Flash
	{
	public:

		virtual ~Flash() = default;

		Flash() = default;

		void ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);
		virtual void ComputeEquilibrium(MultiphaseSystemProperties& out_variables)=0;
		virtual void set_PhaseState(MultiphaseSystemProperties& out_variables)=0;
		

	protected:

		void ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables);
	};
}
