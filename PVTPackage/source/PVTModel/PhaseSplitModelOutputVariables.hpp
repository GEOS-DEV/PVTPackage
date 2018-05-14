#pragma once
#include <unordered_map>
#include "PVTModel/PVTEnums.hpp"
#include "PhaseSplitModel/CompositionalFlash.hpp"

namespace PVTPackage
{
	struct PhaseSplitModelOutputVariables
	{
		PhaseSplitModelOutputVariables() : PhaseSplitModel(nullptr), PhaseState(PHASE_STATE::UNKNOWN)
		{
		}

		//--Vector of Phase composition (mol/mol) of 'phase' 
		void set_PhaseState()
		{
			PhaseState = PhaseStateMap.at({ MoleFraction[PHASE_TYPE::OIL] > 0., MoleFraction[PHASE_TYPE::GAS] > 0. , MoleFraction[PHASE_TYPE::LIQUID_WATER_RICH] > 0. });
		}

		const CubicEoSPhaseModel* PhaseSplitModel;
		PHASE_STATE PhaseState;
		std::unordered_map<PHASE_TYPE, std::vector<double>> MoleComposition;
		std::unordered_map<PHASE_TYPE, double> MoleFraction;

	};

}



