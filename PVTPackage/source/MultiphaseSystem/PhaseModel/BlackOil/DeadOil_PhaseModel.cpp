#include "DeadOil_PhaseModel.hpp"
#include <algorithm>
#include "Utils/math.hpp"
#include "BlackOil_Utils.hpp"
#include <algorithm>    // std::max
#include <complex>
#include "MultiphaseSystem/PhaseModel/PhaseProperties.hpp"

namespace PVTPackage
{

	DeadOil_PhaseModel::DeadOil_PhaseModel(PHASE_TYPE type, std::vector<std::vector<double>> PVD, double surface_mass_density, double surface_mw)
  :	m_type(type), m_SurfaceMassDensity(0), m_SurfaceMoleDensity(0), m_SurfaceMolecularWeight(surface_mw)
{

  //--Fill table
  CreateTable(PVD);

  //Check Consistency
  CheckTableConsistency();

  //Check Consistency
  CheckTableConsistency();

  //Density
  m_SurfaceMassDensity = surface_mass_density;
  m_SurfaceMoleDensity = m_SurfaceMassDensity / m_SurfaceMolecularWeight;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void DeadOil_PhaseModel::CheckTableConsistency()
{
  //--Check for the presence of two pressure
  ASSERT(m_PVD.Pressure.size() > 1, "At least two pressure is required");


  //Check for saturated region
  for (size_t i = 0; i < m_PVD.NPoints - 1; ++i)
  {
	  if (m_type==PHASE_TYPE::OIL)
	  {
		  //Bo must increase with P
		  ASSERT((m_PVD.B[i + 1] - m_PVD.B[i]) > 0, "Bo must increase with P");
		  //Visc must decrease with P
		  ASSERT((m_PVD.Viscosity[i + 1] - m_PVD.Viscosity[i]) < 0, "Viscosity must increase with P");
	  }
	  else if (m_type == PHASE_TYPE::GAS)
	  {
		  //Bg must decrease with P
		  ASSERT((m_PVD.B[i + 1] - m_PVD.B[i]) < 0, "Bo must increase with P");
		  //Visc must increase with P
		  ASSERT((m_PVD.Viscosity[i + 1] - m_PVD.Viscosity[i]) > 0, "Viscosity must increase with P");
	  }

   
  }


}

void DeadOil_PhaseModel::CreateTable(const std::vector<std::vector<double>>& PVD)
{
  m_PVD.NPoints = PVD.size()-1;

  for (size_t i = 0; i < PVD.size(); ++i)
  {
    m_PVD.Pressure.resize(PVD.size());
    m_PVD.B.resize(PVD.size());
    m_PVD.Viscosity.resize(PVD.size());

    //Saturated part
	m_PVD.Pressure[i]=PVD[i][0];
	m_PVD.B[i]=PVD[i][1];
	m_PVD.Viscosity[i]=PVD[i][2];
  }

  //Add 1atm value if does not exist yet
  auto Pref = 101325.0;
  if (!math::isNearlyEqual(m_PVD.Pressure[0], Pref))
  {
    auto visc = math::LogExtrapolation(m_PVD.Pressure[1], m_PVD.Viscosity[1], m_PVD.Pressure[0], m_PVD.Viscosity[0], Pref);
    m_PVD.Pressure.insert(m_PVD.Pressure.begin(), Pref);
    m_PVD.NPoints++;
  }

  //
  max_Pressure = *(std::max_element(m_PVD.Pressure.begin(), m_PVD.Pressure.end()));
  min_Pressure = *(std::min_element(m_PVD.Pressure.begin(), m_PVD.Pressure.end()));

}


void DeadOil_PhaseModel::ComputeProperties(double P, PhaseProperties& props_out) const
{
  ASSERT((P < max_Pressure)&(P > min_Pressure), "Pressure out of table range");

  if (m_type == PHASE_TYPE::OIL)
  {
	  props_out.MoleComposition.value = { 1.,0.,0. };
  }
  else if (m_type == PHASE_TYPE::GAS)
  {
	  props_out.MoleComposition.value = { 0.,1.,0. };
  }
  double B, Visc;
  ComputeBandVisc(P, B, Visc);
  props_out.Viscosity = Visc;
  props_out.MoleDensity.value = ComputeMoleDensity(B);
  props_out.MassDensity.value = ComputeMassDensity(B);
  props_out.MolecularWeight.value = props_out.MassDensity.value / props_out.MoleDensity.value;
}

void DeadOil_PhaseModel::ComputeBandVisc(double P, double& B, double& visc) const
{
  ASSERT((P<max_Pressure)&(P>min_Pressure), "Pressure out of table range");
  size_t i_lower_branch, i_upper_branch;
  auto P_vec = m_PVD.Pressure;
  auto& B_vec = m_PVD.B;
  auto& visc_vec = m_PVD.Viscosity;
  math::FindSurrondingIndex(P_vec, P, i_lower_branch, i_upper_branch);
  B = math::LinearInterpolation(P - P_vec[i_lower_branch], P_vec[i_upper_branch] - P, B_vec[i_lower_branch], B_vec[i_upper_branch]);
  visc = math::LinearInterpolation(P - P_vec[i_lower_branch], P_vec[i_upper_branch] - P, visc_vec[i_lower_branch], visc_vec[i_upper_branch]);
}

double DeadOil_PhaseModel::ComputeMoleDensity(double B) const
{
  return 1. / B*(m_SurfaceMoleDensity );
}

double DeadOil_PhaseModel::ComputeMassDensity(double B) const
{
  return 1. / B * (m_SurfaceMassDensity );
}

}
