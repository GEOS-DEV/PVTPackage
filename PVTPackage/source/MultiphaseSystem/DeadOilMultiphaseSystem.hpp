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

#include "MultiphaseSystem/MultiphaseSystemProperties/DeadOilFlashMultiphaseSystemProperties.hpp"
#include "PhaseSplitModel/DeadOilFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

#include <vector>

namespace PVTPackage
{

class DeadOilMultiphaseSystem final : public MultiphaseSystem, private TableReader
{
public:

  static std::unique_ptr< DeadOilMultiphaseSystem > build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                           const std::vector< std::string > & tableFileNames,
                                                           const std::vector< double > & surfaceDensities,
                                                           const std::vector< double > & molarWeights );

  virtual void Update( double pressure, double temperature, std::vector< double > feed ) override;

  const pvt::MultiphaseSystemProperties & getMultiphaseSystemProperties() const override;

private:

  DeadOilMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                           const std::vector< std::vector< double > > & PVDO,
                           double oilSurfaceMassDensity,
                           double oilSurfaceMolecularWeight,
                           const std::vector< std::vector< double > > & PVDG,
                           double gasSurfaceMassDensity,
                           double gasSurfaceMolecularWeight,
                           const std::vector< double > & PVTW,
                           double waterSurfaceMassDensity,
                           double waterSurfaceMolecularWeight );

  DeadOilFlash m_deadOilFlash;

  DeadOilFlashMultiphaseSystemProperties m_dofmsp;
};

}
