#pragma once
#include "PVTModel/Flash/CompositionalFlash.hpp"

namespace PVTPackage
{

	class TrivialFlash final : public CompositionalFlash
	{
	public:

		TrivialFlash(std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types, ComponentProperties* comp_props)
			: CompositionalFlash(phase_types, eos_types, comp_props)
		{
		}

		void ComputeEquilibrium(const FlashInputVariables& in_variables, FlashOutputVariables& out_variables) override;

	protected:



	};

}


