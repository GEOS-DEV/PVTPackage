/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 TotalEnergies
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include "MultiphaseSystem/MultiphaseSystemProperties/BlackOilFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "PhaseSplitModel/BlackOilFlash.hpp"

#include "pvt/pvt.hpp"

#include <vector>

namespace PVTPackage
{

class BlackOilMultiphaseSystem final : public MultiphaseSystem, private TableReader
{
public:

  static std::unique_ptr< BlackOilMultiphaseSystem > build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                            const std::vector< std::string > & tableFileNames,
                                                            const std::vector< double > & surfaceMassDensities,
                                                            const std::vector< double > & molarWeights );

  void Update( double pressure,
               double temperature,
               std::vector< double > feed ) override;

  const pvt::MultiphaseSystemProperties & getMultiphaseSystemProperties() const override;

private:

  BlackOilMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                            const std::vector< std::vector< double > > & PVTO,
                            double oilSurfaceMassDensity,
                            double oilSurfaceMolecularWeight,
                            const std::vector< std::vector< double > > & PVTG,
                            double gasSurfaceMassDensity,
                            double gasSurfaceMolecularWeight,
                            const std::vector< double > & PVTW,
                            double waterSurfaceMassDensity,
                            double waterSurfaceMolecularWeight );

  BlackOilFlash m_blackOilFlash;

  BlackOilFlashMultiphaseSystemProperties m_bofmsp;
};

}
