#pragma once
#include <unordered_map>
#include "MultiphaseSystem/PVTEnums.hpp"
#include <list>


namespace PVTPackage
{
	struct MultiphaseSystemProperties;
	class PhaseModel;

	class BlackOilFlash
	{
	public:

		BlackOilFlash();

		virtual ~BlackOilFlash() = default;

		void ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);
		void set_PhaseState(MultiphaseSystemProperties& out_variables);
		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables);

	protected:

		PhaseModel* m_OilModel;
		PhaseModel* m_GasModel;
		PhaseModel* m_WaterModel;

	};
}
