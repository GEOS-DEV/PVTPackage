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

		BlackOilFlash(std::list<PHASE_TYPE> phase_types);

		virtual void ComputeEquilibrium(double pressure, double temperature, std::vector<double> feed, MultiphaseSystemProperties* out_variables)=0;

	protected:

		PhaseModel* m_OilModel;
		PhaseModel* m_GasModel;
		PhaseModel* m_WaterModel;

	};
}
