#pragma once
#include <unordered_map>

namespace PVTPackage
{
	class CubicEoSPhaseModel;

	struct PhaseModelOutputVariables
	{
		PhaseModelOutputVariables(const CubicEoSPhaseModel* phase_model): m_PhaseModel(phase_model),
		                                                                  CompressibilityFactor(-1),
		                                                                  MoleDensity(-1), MassDensity(-1)
		{
		}

		const CubicEoSPhaseModel* m_PhaseModel;
		double CompressibilityFactor;
		double MoleDensity;
		double MassDensity;
		double MolecularWeight;
		std::vector<double> LnFugacityCoefficients;

	};

}



