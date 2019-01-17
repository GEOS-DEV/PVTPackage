#pragma once

#include "Utils/Assert.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <map>
#include "PVDdata.hpp"


namespace PVTPackage
{

class DeadOil_PhaseModel final : public PhaseModel
{
public:

	DeadOil_PhaseModel(PHASE_TYPE type, std::vector<std::vector<double>> PVD, double oil_surface_mass_density, double oil_surface_mw);

  ~DeadOil_PhaseModel() override = default;

  //Getter
  double GetSurfaceMassDensity() { return m_SurfaceMassDensity; }
  double GetSurfaceMoleDensity() { return m_SurfaceMoleDensity; }
  double GetSurfaceMolecularWeight() { return m_SurfaceMolecularWeight; }

  //Compute
  void ComputeProperties(double P, PhaseProperties& props_out) const;
 

protected:


  //Phase type
	PHASE_TYPE m_type;

  //PVT data
  PVDdata m_PVD;

  //
  double min_Pressure{};
  double max_Pressure{};

  //
  double m_SurfaceMassDensity;
  double m_SurfaceMoleDensity;
  double m_SurfaceMolecularWeight;

  //
  void ComputeBandVisc(double P, double& B, double& visc) const;
  double ComputeMoleDensity(double B) const;
  double ComputeMassDensity(double B) const;


  //Functions
  void CreateTable(const std::vector<std::vector<double>>& PVD);
  void CheckTableConsistency();

};

}
