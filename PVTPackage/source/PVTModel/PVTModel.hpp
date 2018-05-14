#pragma once
#include <vector>
#include "PVTModel/PVTEnums.hpp"

namespace PVTPackage
{
	struct PhaseSplitModelOutputVariables;

	class PVTModel
	{
	public:
		virtual ~PVTModel();

		PVTModel(size_t nc, std::vector<PHASE_TYPE> phase_types);

		// Compute secondary variables from phase equilibrium
		virtual void Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables)=0;

	protected:

		//Dimensions
		size_t m_NComponents;
		size_t m_NMaxPhases; 

		//Phases
		std::vector<PHASE_TYPE> m_PhaseTypes;

	};

}
