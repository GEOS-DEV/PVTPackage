#pragma once

#include "Flash.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <unordered_map>
#include <list>
#include "Flash.hpp"


namespace PVTPackage
{
	struct MultiphaseSystemProperties;
	class PhaseModel;

<<<<<<< HEAD
	class BlackOilFlash :public Flash
=======
	class BlackOilFlash : public Flash
>>>>>>> 489cf64f5560b9a2d2f0f4030fb7292341915c23
	{
	public:

		BlackOilFlash();
<<<<<<< HEAD
		void ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;
		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;
=======

		~BlackOilFlash() override = default;

		void set_PhaseState(MultiphaseSystemProperties& out_variables) override;
		bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;
>>>>>>> 489cf64f5560b9a2d2f0f4030fb7292341915c23

	protected:

		PhaseModel* m_OilModel;
		PhaseModel* m_GasModel;
		PhaseModel* m_WaterModel;

	};
}
