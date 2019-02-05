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
		double GetSurfaceMassDensity() { return m_SurfaceMassDensity; }
		double GetSurfaceMoleDensity() { return m_SurfaceMoleDensity; }
		double GetSurfaceMolecularWeight() { return m_SurfaceMolecularWeight; }

		//Compute
		double ComputeRs(double Pb);
		void ComputeSaturatedProperties(double Pb, std::vector<double> composition, double gas_mole_surface_density, double gas_mass_surface_density, PhaseProperties& props_out);
		void ComputeUnderSaturatedProperties(double P, std::vector<double> composition, double gas_mole_surface_density, double gas_mass_surface_density, PhaseProperties& props_out);
		
		//HQPAN
		void calcRS(double pres, double &RS, double &dRS_dp);


	protected:


		//PVT data
		PVTOdata m_PVTO;

		//
		double min_Pressure;
		double max_Pressure;

		//
		double m_SurfaceMassDensity;
		double m_SurfaceMoleDensity;
		double m_SurfaceMolecularWeight;

		//
		double ComputePb(double Rs);
		void ComputeSaturatedBoVisc(double Rs, double& Bo, double& visc) const;
		void ComputeUndersaturatedBoVisc(double Rs, double P, double& Bo, double& visc);
		double ComputeMoleDensity(double Rs, double Bo, double surface_gas_mole_density) const;
		double ComputeMassDensity(double Rs, double Bo, double surface_gas_mass_density) const;


		//Functions
		void CreateTable(const std::vector<std::vector<double>>& PVT);
		void ExtendUnderSaturatedProperties();
		void CreateUnderSaturatedProperties();
		void CheckTableConsistency();
		void RefineTable(size_t nlevel);

		


	};

	
}
