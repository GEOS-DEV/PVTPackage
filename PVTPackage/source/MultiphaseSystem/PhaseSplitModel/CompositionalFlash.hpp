#pragma once
#include "MultiphaseSystem/PhaseModel/CubicEoSPhaseModel.hpp"
#include <unordered_map>
#include "MultiphaseSystem/PVTEnums.hpp"


namespace PVTPackage
{
	struct MultiphaseSystemProperties;

	class CompositionalFlash
	{
	public:
		virtual ~CompositionalFlash();

		CompositionalFlash(std::unordered_map<PHASE_TYPE, PhaseModel*>& phase_models);

		virtual void ComputeEquilibrium(double pressure, double temperature, std::vector<double> feed, MultiphaseSystemProperties* out_variables)=0;

	protected:
		std::unordered_map<PHASE_TYPE, PhaseModel*> m_PhaseModels;

		//Component properties
		const ComponentProperties* m_ComponentsProperties;

		//Wilson K-values
		std::vector<double> ComputeWilsonGasOilKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterGasKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterOilKvalue(double Pressure, double Temperature) const;

	};
}
