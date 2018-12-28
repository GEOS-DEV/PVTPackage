#include "BlackOil_WaterModel.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseProperties.hpp"
#include <math.h>
#include <cmath>
namespace PVTPackage
{
	
	BlackOil_WaterModel::BlackOil_WaterModel(std::vector<double> PVTW, double surface_density, double mw):
		m_SurfaceMassDensity(surface_density),m_SurfaceMolecularWeight(mw)
	{
		m_PVTW.ReferencePressure = PVTW[0];
		m_PVTW.Bw = PVTW[1];
		m_PVTW.Compressibility = PVTW[2];
		m_PVTW.Viscosity = PVTW[3];

		//Density
		m_SurfaceMoleDensity = m_SurfaceMassDensity / m_SurfaceMolecularWeight;
	}

	void BlackOil_WaterModel::ComputeProperties(double P, PhaseProperties& props_out)
	{
		props_out.MoleComposition.value = {0.,0.,1};
		props_out.MolecularWeight.value = m_SurfaceMolecularWeight;
		props_out.MassDensity.value = m_SurfaceMassDensity / (m_PVTW.Bw * std::exp(-m_PVTW.Compressibility*(P - m_PVTW.ReferencePressure)));
		props_out.MoleDensity.value = props_out.MassDensity.value*props_out.MolecularWeight.value;
		props_out.Viscosity = m_PVTW.Viscosity;
	}
}
