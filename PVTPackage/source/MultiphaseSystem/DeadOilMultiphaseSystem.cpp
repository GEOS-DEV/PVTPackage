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

#include "MultiphaseSystem/DeadOilMultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

DeadOilMultiphaseSystem::DeadOilMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                                  const std::vector< std::vector< double > > & PVDO,
                                                  double oilSurfaceMassDensity,
                                                  double oilSurfaceMolecularWeight,
                                                  const std::vector< std::vector< double > > & PVDG,
                                                  double gasSurfaceMassDensity,
                                                  double gasSurfaceMolecularWeight,
                                                  const std::vector< double > & PVTW,
                                                  double waterSurfaceMassDensity,
                                                  double waterSurfaceMolecularWeight )
  :
  m_dofmsp( phases.size(),
            PVDO, oilSurfaceMassDensity, oilSurfaceMolecularWeight,
            PVDG, gasSurfaceMassDensity, gasSurfaceMolecularWeight,
            PVTW, waterSurfaceMassDensity, waterSurfaceMolecularWeight )
{

}

void DeadOilMultiphaseSystem::Update( double pressure,
                                      double temperature,
                                      std::vector< double > feed )
{
  // Temperature unused
  (void) temperature;
  m_dofmsp.setPressure( pressure );
  m_dofmsp.setFeed( feed );

  const bool result = computeDerivativesNoTemperature( m_deadOilFlash, m_dofmsp );

  m_stateIndicator = result ? State::SUCCESS : State::NOT_CONVERGED;
}

const pvt::MultiphaseSystemProperties & DeadOilMultiphaseSystem::getMultiphaseSystemProperties() const
{
  return m_dofmsp;
}

std::unique_ptr< DeadOilMultiphaseSystem > DeadOilMultiphaseSystem::build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                           const std::vector< std::string > & tableFileNames,
                                                                           const std::vector< double > & surfaceDensities,
                                                                           const std::vector< double > & molarWeights )
{
  // props.oilTable, props.gasTable and props.waterTable respectively contain PVDO, PVDG and PVTW
  const Properties & props = buildTables( phases, tableFileNames, surfaceDensities, molarWeights );

  // I am not using std::make_unique because I want the constructor to be private.
  auto * ptr = new DeadOilMultiphaseSystem( phases,
                                            props.oilTable, props.oilSurfaceMassDensity, props.oilSurfaceMolecularWeight,
                                            props.gasTable, props.gasSurfaceMassDensity, props.gasSurfaceMolecularWeight,
                                            props.waterTable, props.waterSurfaceMassDensity, props.waterSurfaceMolecularWeight );
  return std::unique_ptr< DeadOilMultiphaseSystem >( ptr );
}

}
