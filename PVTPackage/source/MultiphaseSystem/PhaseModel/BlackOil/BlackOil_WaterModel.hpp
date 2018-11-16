#pragma once
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include "PVTWdata.hpp"


namespace PVTPackage
{

	class BlackOil_WaterModel final : public PhaseModel
	{
	public:

		BlackOil_WaterModel(std::vector<double> PVTW, double water_surface_mass_density, double water_surface_mw);

		~BlackOil_WaterModel() override = default;

		//Getter
		double GetSurfaceMassDensity() { return m_SurfaceMassDensity; }
		double GetSurfaceMoleDensity() { return m_SurfaceMoleDensity; }
		double GetSurfaceMolecularWeight() { return m_SurfaceMolecularWeight; }

		//Compute
		void ComputeProperties(double P, PhaseProperties& props_out);



	protected:


		//PVT data
		PVTWdata m_PVTW{};


		//
		double m_SurfaceMassDensity;
		double m_SurfaceMoleDensity;
		double m_SurfaceMolecularWeight;

	};


}
