/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 Total, S.A
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "TrivialMultiphaseSystem.hpp"

namespace PVTPackage
{

std::unique_ptr< TrivialMultiphaseSystem > TrivialMultiphaseSystem::build( std::vector< pvt::PHASE_TYPE > const & phases,
                                                                           std::vector< pvt::EOS_TYPE > const & eosTypes,
                                                                           std::vector< std::string > const & componentNames,
                                                                           std::vector< double > const & componentMolarWeights,
                                                                           std::vector< double > const & componentCriticalTemperatures,
                                                                           std::vector< double > const & componentCriticalPressures,
                                                                           std::vector< double > const & componentOmegas )
{
  if( not areComponentDataConsistent( componentNames,
                                      componentMolarWeights,
                                      componentCriticalTemperatures,
                                      componentCriticalPressures,
                                      componentOmegas ) )
  {
    return std::unique_ptr< TrivialMultiphaseSystem >();
  }

  ComponentProperties cp( componentNames.size(),
                          componentNames,
                          componentMolarWeights,
                          componentCriticalTemperatures,
                          componentCriticalPressures,
                          componentOmegas );

  // I am not using std::make_unique because I want the constructor to be private.
  auto * ptr = new TrivialMultiphaseSystem( phases, eosTypes, cp );
  return std::unique_ptr< TrivialMultiphaseSystem >( ptr );
}

TrivialMultiphaseSystem::TrivialMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                                  const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                  const ComponentProperties & componentProperties )
  :
  m_trivialFlash( phases, eosTypes, componentProperties ),
  m_tfmsp( phases, componentProperties.NComponents )
{

}

void TrivialMultiphaseSystem::Update( double pressure,
                                      double temperature,
                                      std::vector< double > feed )
{
  m_tfmsp.setPressure( pressure );
  m_tfmsp.setTemperature( temperature );
  m_tfmsp.setFeed( feed );

  const bool result = computeEquilibriumAndDerivativesWithTemperature( m_trivialFlash, m_tfmsp );

  m_stateIndicator = result ? State::SUCCESS : State::NOT_CONVERGED;
}

const pvt::MultiphaseSystemProperties & TrivialMultiphaseSystem::getMultiphaseSystemProperties() const
{
  return m_tfmsp;
}

}