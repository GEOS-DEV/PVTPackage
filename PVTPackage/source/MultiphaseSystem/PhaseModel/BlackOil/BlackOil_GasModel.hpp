#pragma once
#include "Utils/Assert.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <map>
#include "PVTGdata.hpp"


namespace PVTPackage
{
	struct PhaseProperties;

	class BlackOil_GasModel final : public PhaseModel
	{
	public:

		BlackOil_GasModel(std::vector<std::vector<double>> PVTG, double gas_surface_mass_density, double gas_surface_mw);
		~BlackOil_GasModel() override = default;

		//Getter
		double GetSurfaceGasMassDensity() { return m_SurfaceGasMassDensity; }
		double GetSurfaceGasMoleDensity() { return m_SurfaceGasMoleDensity; }
		double GetSurfaceGasMolecularWeight() { return m_SurfaceGasMolecularWeight; }

		//
		double ComputeRv(double Pdew);
		void ComputeSaturatedProperties(double Pdew, std::vector<double> composition, double oil_mole_surface_density, double oil_mass_surface_density, PhaseProperties& props_out);
		void ComputeUnderSaturatedProperties(double P, std::vector<double> composition, double oil_mole_surface_density, double oil_mass_surface_density, PhaseProperties& props_out);


	protected:

		//PVT data
		PVTGdata m_PVTG;

		//		
		double m_SurfaceGasMassDensity;
		double m_SurfaceGasMoleDensity;
		double m_SurfaceGasMolecularWeight;


		//
		double ComputePdew(double Rv);
		void ComputeSaturatedBgVisc(double Rv, double& Bg, double& visc) const;
		void ComputeUndersaturatedBgVisc(double Rv, double P, double& Bg, double& visc);

		double ComputeMoleDensity(double Rv, double Bg, double surface_oil_mole_density) const;
		double ComputeMassDensity(double Rv, double Bg, double surface_oil_mass_density) const;


		//Functions
		void CreateTable(const std::vector<std::vector<double>>& PVT);
		void ExtendUnderSaturatedProperties();
		void CheckTableConsistency();
		void RefineTable(size_t nlevel);

		


	};

	
}
