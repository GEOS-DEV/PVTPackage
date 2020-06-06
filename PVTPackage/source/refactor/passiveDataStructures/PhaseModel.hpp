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

#ifndef PVTPACKAGE_PDS_PHASEMODEL_HPP
#define PVTPACKAGE_PDS_PHASEMODEL_HPP

#include "refactor/passiveDataStructures/PVTEnums.hpp"
#include "refactor/passiveDataStructures/ComponentProperties.hpp"

#include <vector>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

class PhaseModel
{
public:

  PhaseModel() {};

  virtual ~PhaseModel() {};

};

class CubicEoSPhaseModel final : public PhaseModel
{
public:
  pds::ComponentProperties m_ComponentsProperties;
  pds::PHASE_TYPE m_PhaseType;
  //EOS parameters
  pds::EOS_TYPE m_EOSType;
  double m_OmegaA;
  double m_OmegaB;
  double m_Delta1, m_Delta2;
  //Constant Properties
  std::vector<double> m_m;
  double m_BICs;
};

class PVTGdata
{
public:
  std::vector<double> Rv;
  std::vector<double> DewPressure;
  std::size_t NSaturatedPoints;
  std::vector<double> SaturatedBg;
  std::vector<double> SaturatedViscosity;
  std::vector<std::vector<double>> UndersaturatedRv;   // always start at 0
  std::vector<std::vector<double>> UndersaturatedBg;
  std::vector<std::vector<double>> UndersaturatedViscosity;
  double MaxRelativeRv;
  double MinRelativeRv;
};

class BlackOilGasModel final : public PhaseModel
{
public:
  PVTGdata m_PVTG;
  double min_Pressure;
  double max_Pressure;
  double m_SurfaceMassDensity;
  double m_SurfaceMoleDensity;
  double m_SurfaceMolecularWeight;
};

class PVTOdata
{
public:
  std::vector<double> Rs;
  std::vector<double> BubblePressure;
  std::size_t NSaturatedPoints;
  std::vector<double> SaturatedBo;
  std::vector<double> SaturatedViscosity;
  std::vector<std::vector<double>> UndersaturatedPressure;
  std::vector<std::vector<double>> UndersaturatedBo;
  std::vector<std::vector<double>> UndersaturatedViscosity;
  double MaxRelativePressure;
  double MinRelativePressure;
};

class BlackOilOilModel final : public PhaseModel
{
public:
  PVTOdata m_PVTO;
  double min_Pressure;
  double max_Pressure;
  double m_SurfaceMassDensity;
  double m_SurfaceMoleDensity;
  double m_SurfaceMolecularWeight;
};

class PVTWdata
{
public:
  double ReferencePressure;
  double Bw;
  double Compressibility;
  double Viscosity;
};

class BlackOilWaterModel final : public PhaseModel
{
public:
  PVTWdata m_PVTW;
  double m_SurfaceMassDensity;
  double m_SurfaceMoleDensity;
  double m_SurfaceMolecularWeight;
};

class PVDdata
{
public:
  std::vector<double> Pressure;
  std::size_t NPoints;
  std::vector<double> B;
  std::vector<double> Viscosity;
};

class DeadOilPhaseModel final : public PhaseModel
{
public:
  PHASE_TYPE m_type;
  PVDdata m_PVD;
  double min_Pressure;
  double max_Pressure;
  double m_SurfaceMassDensity;
  double m_SurfaceMoleDensity;
  double m_SurfaceMolecularWeight;
};

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_PDS_PHASEMODEL_HPP
