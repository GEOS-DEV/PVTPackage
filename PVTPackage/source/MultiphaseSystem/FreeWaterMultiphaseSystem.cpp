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

#include "FreeWaterMultiphaseSystem.hpp"

namespace PVTPackage
{

std::unique_ptr< FreeWaterMultiphaseSystem > FreeWaterMultiphaseSystem::build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                               const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                                               const std::vector< std::string > & componentNames,
                                                                               const std::vector< double > & componentMolarWeights,
                                                                               const std::vector< double > & componentCriticalTemperatures,
                                                                               const std::vector< double > & componentCriticalPressures,
                                                                               const std::vector< double > & componentOmegas )
{
  if( not areComponentDataConsistent( componentNames,
                                      componentMolarWeights,
                                      componentCriticalTemperatures,
                                      componentCriticalPressures,
                                      componentOmegas ) )
  {
    return std::unique_ptr< FreeWaterMultiphaseSystem >();
  }

  ComponentProperties cp( componentNames.size(),
                          componentNames,
                          componentMolarWeights,
                          componentCriticalTemperatures,
                          componentCriticalPressures,
                          componentOmegas );

  // I am not using std::make_unique because I want the constructor to be private.
  auto * ptr = new FreeWaterMultiphaseSystem( phases, eosTypes, cp );
  return std::unique_ptr< FreeWaterMultiphaseSystem >( ptr );
}

FreeWaterMultiphaseSystem::FreeWaterMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                                      const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                      const ComponentProperties & componentProperties )
  :
  m_freeWaterFlash( componentProperties ),
  m_fwfmsp( phases, eosTypes, componentProperties )
{

}

const pvt::MultiphaseSystemProperties & FreeWaterMultiphaseSystem::getMultiphaseSystemProperties() const
{
  return m_fwfmsp;
}

void FreeWaterMultiphaseSystem::Update( double pressure,
                                        double temperature,
                                        std::vector< double > feed )
{
  m_fwfmsp.setTemperature( temperature );
  m_fwfmsp.setPressure( pressure );
  m_fwfmsp.setFeed( feed );

  const bool result = computeDerivativesWithTemperature( m_freeWaterFlash, m_fwfmsp );

  m_stateIndicator = result ? State::SUCCESS : State::NOT_CONVERGED;
}

}
