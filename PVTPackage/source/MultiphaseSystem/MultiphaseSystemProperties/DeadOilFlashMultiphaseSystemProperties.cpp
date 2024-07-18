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

#include "DeadOilFlashMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

#include <cmath>
#include <algorithm>

namespace PVTPackage
{

DeadOilFlashMultiphaseSystemProperties::DeadOilFlashMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases )
  :
  BlackOilDeadOilMultiphaseSystemProperties( phases )
{
  size_t const nComponents = getNComponents();
  
  for( pvt::PHASE_TYPE pt: getPhases() )
  {
    m_lnFugacity.insert( { pt, pvt::VectorPropertyAndDerivatives< double >( nComponents, nComponents ) } );
  }

  size_t const nPhases = phases.size();

  // TODO: ultimately, all these if statements should go away
  
  if( nPhases == 3 )
  {
    m_moleComposition.at( pvt::PHASE_TYPE::OIL ).value = { 1., 0., 0. };
    m_moleComposition.at( pvt::PHASE_TYPE::GAS ).value = { 0., 1., 0. };
    m_moleComposition.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { 0., 0., 1. };

    m_lnFugacity.at( pvt::PHASE_TYPE::OIL ).value = { std::log( 1. ), std::log( 1. ), std::log( 1. ) };
    m_lnFugacity.at( pvt::PHASE_TYPE::GAS ).value = { std::log( 1. ), std::log( 1. ), std::log( 1. ) };
    m_lnFugacity.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { std::log( 1. ), std::log( 1. ), std::log( 1. ) };
  }
  else // nPhases = 2 
  {
    // the system is either oil-water or oil-gas
    
    m_moleComposition.at( pvt::PHASE_TYPE::OIL ).value = { 1., 0. };
    m_lnFugacity.at( pvt::PHASE_TYPE::OIL ).value = { std::log( 1. ), std::log( 1. ) };

    bool const containsGas = std::find( phases.cbegin(), phases.cend(), pvt::PHASE_TYPE::GAS ) != phases.cend();
    if( containsGas )
    {      
      m_moleComposition.at( pvt::PHASE_TYPE::GAS ).value = { 0., 1. };
      m_lnFugacity.at( pvt::PHASE_TYPE::GAS ).value = { std::log( 1. ), std::log( 1. ) };
    }
    else
    {     
      m_moleComposition.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { 0., 1. };
      m_lnFugacity.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { std::log( 1. ), std::log( 1. ) };
    }
  }
}

double DeadOilFlashMultiphaseSystemProperties::getOilPhaseMoleFraction() const
{
  return m_phaseMoleFraction.at( pvt::PHASE_TYPE::OIL ).value;
}

double DeadOilFlashMultiphaseSystemProperties::getGasPhaseMoleFraction() const
{
  return m_phaseMoleFraction.at( pvt::PHASE_TYPE::GAS ).value;
}

double DeadOilFlashMultiphaseSystemProperties::getWaterPhaseMoleFraction() const
{
  return m_phaseMoleFraction.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value;
}

void DeadOilFlashMultiphaseSystemProperties::setFeed( std::vector< double > const & feed )
{
  FactorMultiphaseSystemProperties::setFeed( feed );

  // FIXME Hard coded indices, use constants
  size_t const nPhases = getPhases().size();

  // TODO: ultimately, all these if statements should go away
  
  m_phaseMoleFraction.at( pvt::PHASE_TYPE::OIL ).value = feed[0];
  if( nPhases == 3 )
  {  
    m_phaseMoleFraction.at( pvt::PHASE_TYPE::GAS ).value = feed[1];
    m_phaseMoleFraction.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = feed[2];
  }
  else // nPhases = 2 
  {
    // the system is either oil-water or oil-gas
    bool const containsGas = std::find( getPhases().cbegin(), getPhases().cend(), pvt::PHASE_TYPE::GAS ) != getPhases().end();
    if( containsGas )
    {
      m_phaseMoleFraction.at( pvt::PHASE_TYPE::GAS ).value = feed[1];
    }
    else
    {
      m_phaseMoleFraction.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = feed[1];
    }
    
  }
}

}
