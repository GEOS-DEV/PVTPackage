#pragma once
#include <unordered_map>
#include "MultiphaseSystem/PVTEnums.hpp"
#include <list>
#include "Flash.hpp"


namespace PVTPackage
{
	struct MultiphaseSystemProperties;
	class PhaseModel;

	class BlackOilFlash :public Flash
	{
	public:

		BlackOilFlash();
		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;
		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;

	protected:

		PhaseModel* m_OilModel;
		PhaseModel* m_GasModel;
		PhaseModel* m_WaterModel;

	};
}
