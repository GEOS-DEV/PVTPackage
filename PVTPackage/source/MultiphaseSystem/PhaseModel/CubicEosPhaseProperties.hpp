#pragma once
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseProperties.hpp"

struct ComponentProperties;

namespace PVTPackage
{

	struct CubicEoSPhaseProperties final : PhaseProperties
	{
		CubicEoSPhaseProperties() :
			PhaseProperties(), CompressibilityFactor(0), LnFugacityCoefficients(0) {}

		double CompressibilityFactor;
		std::vector<double> LnFugacityCoefficients;
	};
}