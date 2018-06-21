#pragma once

namespace PVTPackage
{

	struct PhaseProperties
	{
		PhaseProperties() : MolecularWeight(0), MoleDensity(0), MassDensity(0), Viscosity(0), MassEnthalpy(0),
		                    Compressibility(0), CompressibilityFactor(0)
		{
		}

		double MolecularWeight;
		double MoleDensity;
		double MassDensity;
		double Viscosity;
		double MassEnthalpy;
		double Compressibility;
		double CompressibilityFactor;
		std::vector<double> LnFugacityCoefficients;
	};


}
