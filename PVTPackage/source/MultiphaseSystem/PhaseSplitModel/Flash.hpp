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

<<<<<<< HEAD
		void ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);
		virtual void ComputeEquilibrium(MultiphaseSystemProperties& out_variables)=0;
		virtual void set_PhaseState(MultiphaseSystemProperties& out_variables)=0;
		
=======
		bool ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);
		virtual bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) = 0;
		virtual void set_PhaseState(MultiphaseSystemProperties& out_variables) = 0;
>>>>>>> 489cf64f5560b9a2d2f0f4030fb7292341915c23

	protected:

		bool ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables);
	};
}
