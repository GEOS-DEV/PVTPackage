/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

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
  double GetSurfaceMassDensity() { return m_SurfaceMassDensity; }
  double GetSurfaceMoleDensity() { return m_SurfaceMoleDensity; }
  double GetSurfaceMolecularWeight() { return m_SurfaceMolecularWeight; }

  //
  double ComputeRv(double Pdew);
  void ComputeSaturatedProperties(double Pdew, std::vector<double> composition, double oil_mole_surface_density, double oil_mass_surface_density, PhaseProperties& props_out);
  void ComputeUnderSaturatedProperties(double P, std::vector<double> composition, double oil_mole_surface_density, double oil_mass_surface_density, PhaseProperties& props_out);


protected:

  //PVT data
  PVTGdata m_PVTG;


  //
  double min_Pressure;
  double max_Pressure;


  //
  double m_SurfaceMassDensity;
  double m_SurfaceMoleDensity;
  double m_SurfaceMolecularWeight;


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
