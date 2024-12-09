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

#include "BlackOilMultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

BlackOilMultiphaseSystem::BlackOilMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                                    const std::vector< std::vector< double > > & PVTO,
                                                    double oilSurfaceMassDensity,
                                                    double oilSurfaceMolecularWeight,
                                                    const std::vector< std::vector< double > > & PVTG,
                                                    double gasSurfaceMassDensity,
                                                    double gasSurfaceMolecularWeight,
                                                    const std::vector< double > & PVTW,
                                                    double waterSurfaceMassDensity,
                                                    double waterSurfaceMolecularWeight )
  :
  m_blackOilFlash( PVTO, oilSurfaceMassDensity, oilSurfaceMolecularWeight,
                   PVTG, gasSurfaceMassDensity, gasSurfaceMolecularWeight,
                   PVTW, waterSurfaceMassDensity, waterSurfaceMolecularWeight ),
  m_bofmsp( phases )
{

}

std::unique_ptr< BlackOilMultiphaseSystem > BlackOilMultiphaseSystem::build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                             const std::vector< std::string > & tableFileNames,
                                                                             const std::vector< double > & surfaceMassDensities,
                                                                             const std::vector< double > & molarWeights )
{
  // TODO Check consistency between PVTO and PVTG
  // props.oilTable, props.gasTable and props.waterTable respectively contain PVTO, PVTG and PVTW
  const Properties & props = buildTables( phases, tableFileNames, surfaceMassDensities, molarWeights );

  // I am not using std::make_unique because I want the constructor to be private.
  auto * ptr = new BlackOilMultiphaseSystem( phases,
                                             props.oilTable, props.oilSurfaceMassDensity, props.oilSurfaceMolecularWeight,
                                             props.gasTable, props.gasSurfaceMassDensity, props.gasSurfaceMolecularWeight,
                                             props.waterTable, props.waterSurfaceMassDensity, props.waterSurfaceMolecularWeight );
  return std::unique_ptr< BlackOilMultiphaseSystem >( ptr );
}

void BlackOilMultiphaseSystem::Update( double pressure,
                                       double temperature,
                                       std::vector< double > feed )
{
  // Temperature unused
  (void) temperature;
  m_bofmsp.setPressure( pressure );
  m_bofmsp.setFeed( feed );

  const bool res = computeEquilibriumAndDerivativesNoTemperature( m_blackOilFlash, m_bofmsp );

  m_stateIndicator = res ? State::SUCCESS : State::NOT_CONVERGED;
}

const pvt::MultiphaseSystemProperties & BlackOilMultiphaseSystem::getMultiphaseSystemProperties() const
{
  return m_bofmsp;
}

}
