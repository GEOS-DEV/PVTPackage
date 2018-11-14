#pragma once
#include "Utils/Assert.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <map>
#include "PVTOdata.hpp"


namespace PVTPackage
{
	
	class BlackOil_OilModel final : public PhaseModel
	{
	public:

		BlackOil_OilModel(std::vector<std::vector<double>> PVTO, double oil_surface_mass_density, double oil_surface_mw);

		~BlackOil_OilModel() override = default;

		//Getter
		double GetSurfaceOilMassDensity() { return m_SurfaceOilMassDensity; }
		double GetSurfaceOilMoleDensity() { return m_SurfaceOilMoleDensity; }
		double GetSurfaceOilMolecularWeight() { return m_SurfaceOilMolecularWeight; }

		//Compute
		double ComputeRs(double Pb);
		void ComputeSaturatedProperties(double Pb, std::vector<double> composition, PhaseProperties& props_out);
		void ComputeUnderSaturatedProperties(double Rs, double P, std::vector<double> composition, PhaseProperties& props_out);
		


	protected:


		//PVT data
		PVTOdata m_PVTO;

		//
		double m_SurfaceOilMassDensity;
		double m_SurfaceOilMoleDensity;
		double m_SurfaceOilMolecularWeight;

		//
		double ComputePb(double Rs);
		void ComputeSaturatedProperties(double Rs, double& Bo, double& visc) const;
		void ComputeUndersaturatedSaturatedProperties(double Rs, double P, double& Bo, double& visc);
		double ComputeMassDensity(double Rs, double Bo, double surface_gas_density) const;
		double ComputeMoleDensity(double mass_density, double mw) const;


		//Functions
		void CreateTable(const std::vector<std::vector<double>>& PVT);
		void ExtendUnderSaturatedProperties();
		void CreateUnderSaturatedProperties();
		void CheckTableConsistency();
		void RefineTable(size_t nlevel);

		


	};

	
}
