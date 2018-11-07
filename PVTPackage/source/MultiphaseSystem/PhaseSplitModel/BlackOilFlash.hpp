#pragma once

#include "Flash.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <unordered_map>
#include <list>


namespace PVTPackage
{
	struct MultiphaseSystemProperties;
	class PhaseModel;

	class BlackOilFlash : public Flash
	{
	public:

		BlackOilFlash();

		~BlackOilFlash() override = default;

		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;
		bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;

	protected:

		PhaseModel* m_OilModel;
		PhaseModel* m_GasModel;
		PhaseModel* m_WaterModel;

	};
}
