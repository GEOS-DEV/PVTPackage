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
		void ComputeSaturatedProperties(double Pdew, std::vector<double> composition, PhaseProperties& props_out);
		void ComputeUnderSaturatedProperties(double Rv, double P, std::vector<double> composition, PhaseProperties& props_out);


	protected:

		//PVT data
		PVTGdata m_PVTG;

		//		
		double m_SurfaceGasMassDensity;
		double m_SurfaceGasMoleDensity;
		double m_SurfaceGasMolecularWeight;


		//
		double ComputePdew(double Rv);
		void ComputeSaturatedProperties(double Rv, double& Bg, double& visc) const;
		//void ComputeUndersaturatedSaturatedProperties(double Rs, double P, double& Bo, double& visc);
		double ComputeMassDensity(double Rv, double Bg, double surface_oil_density) const;
		double ComputeMoleDensity(double mass_density, double mw) const;


		//Functions
		void CreateTable(const std::vector<std::vector<double>>& PVT);
		void ExtendUnderSaturatedProperties();
		void CheckTableConsistency();
		void RefineTable(size_t nlevel);

		


	};

	
}
