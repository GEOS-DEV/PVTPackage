#pragma once
#include "Utils/Assert.hpp"
#include <vector>

namespace PVTPackage
{


	struct PhaseProperties
	{
		PhaseProperties(): MolecularWeight(0), MoleDensity(0), MassDensity(0), Viscosity(0), MassEnthalpy(0),
		                   Compressibility(0)
		{
		}

		double MolecularWeight;
		double MoleDensity; 
		double MassDensity;
		double Viscosity;
		double MassEnthalpy;
		double Compressibility;
	};


	class Phase
	{
	public:

		Phase(): m_PhaseProperties(nullptr)
		{
		}

		virtual PhaseProperties* ComputeAllProperties (double Pressure, double Temperature, std::vector<double>& composition)=0;

	protected:

		PhaseProperties* m_PhaseProperties;

	};
}
