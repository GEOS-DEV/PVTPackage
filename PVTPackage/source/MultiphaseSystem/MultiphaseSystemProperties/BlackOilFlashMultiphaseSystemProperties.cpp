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

#include "BlackOilFlashMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

BlackOilFlashMultiphaseSystemProperties::BlackOilFlashMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases )
  :
  BlackOilDeadOilMultiphaseSystemProperties( phases )
{
  size_t const nComponents = getNComponents();
  
  for( pvt::PHASE_TYPE pt: getPhases() )
  {
    m_lnFugacity.insert( { pt, pvt::VectorPropertyAndDerivatives< double >( nComponents, nComponents ) } );
  }
  
  m_moleComposition.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { 0., 0., 1. };
}

void BlackOilFlashMultiphaseSystemProperties::setOilFraction( double const & fraction )
{
  setPhaseMoleFraction(pvt::PHASE_TYPE::OIL, fraction );
}

void BlackOilFlashMultiphaseSystemProperties::setGasFraction( double const & fraction )
{
  setPhaseMoleFraction(pvt::PHASE_TYPE::GAS, fraction );
}

void BlackOilFlashMultiphaseSystemProperties::setWaterFraction( double const & fraction )
{
  setPhaseMoleFraction(pvt::PHASE_TYPE::LIQUID_WATER_RICH, fraction );
}

void BlackOilFlashMultiphaseSystemProperties::setOilMoleComposition( std::vector< double > const & moleComposition )
{
  setMoleComposition( pvt::PHASE_TYPE::OIL, moleComposition );
}

void BlackOilFlashMultiphaseSystemProperties::setGasMoleComposition( std::vector< double > const & moleComposition )
{
  setMoleComposition( pvt::PHASE_TYPE::GAS, moleComposition );
}

} // end of namespace PVTPackage
