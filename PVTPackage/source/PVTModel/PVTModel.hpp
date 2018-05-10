#pragma once
#include <vector>
#include "PVTModel/PVTEnums.hpp"
#include "PVTModel/FlashVariables.hpp"

namespace PVTPackage
{

	class PVTModel
	{
	public:
		virtual ~PVTModel();

		PVTModel(size_t nc, std::vector<PHASE_TYPE> phase_types);

		// Compute secondary variables from phase equilibrium
		virtual void Flash(const FlashInputVariables& in_variables, FlashOutputVariables& out_variables)=0;

	protected:

		//Dimensions
		size_t m_NComponents;
		size_t m_NMaxPhases; 

		//Phases
		std::vector<PHASE_TYPE> m_PhaseTypes;

	};

}
