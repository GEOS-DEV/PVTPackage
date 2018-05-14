#pragma once
#include "PVTModel/PhaseModel/CubicEoSPhaseModel.hpp"
#include <unordered_map>
#include "PVTModel/PVTEnums.hpp"
#include "PVTModel/PhaseSplitModelOutputVariables.hpp"

namespace PVTPackage
{
	class CompositionalFlash
	{
	public:
		virtual ~CompositionalFlash();

		CompositionalFlash(std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
		                   ComponentProperties* comp_props, std::unordered_map<PHASE_TYPE, CubicEoSPhaseModel*>* phase_models);

		virtual void ComputeEquilibrium(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables)=0;

	protected:

		std::vector<PHASE_TYPE> m_PhaseTypes;
		std::vector<EOS_TYPE> m_EoSTypes;
		const ComponentProperties* m_ComponentsProperties;
		const std::unordered_map<PHASE_TYPE, CubicEoSPhaseModel*>* m_PhaseModel;

		//Wilson K-values
		std::vector<double> ComputeWilsonGasOilKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterGasKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterOilKvalue(double Pressure, double Temperature) const;

	};
}