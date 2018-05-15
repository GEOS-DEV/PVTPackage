#pragma once
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

namespace PVTPackage
{

	class TrivialFlash final : public CompositionalFlash
	{
	public:

		TrivialFlash(std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types, ComponentProperties* comp_props, std::unordered_map<PHASE_TYPE, CubicEoSPhase*>* phase_models)
			: CompositionalFlash(phase_types, eos_types, comp_props,phase_models)
		{
		}

		void ComputeEquilibrium(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables) override;

	protected:


	};

}


