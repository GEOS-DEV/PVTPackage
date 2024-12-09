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

#include "TrivialFlashMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

TrivialFlashMultiphaseSystemProperties::TrivialFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                std::size_t nComponents )
  :
  CompositionalMultiphaseSystemProperties( phases, nComponents )
{

}

void TrivialFlashMultiphaseSystemProperties::setWaterMoleComposition( std::vector< double > const & moleComposition )
{
  setMoleComposition( pvt::PHASE_TYPE::LIQUID_WATER_RICH, moleComposition );
}

void TrivialFlashMultiphaseSystemProperties::setWaterFraction( double const & fraction )
{
  setPhaseMoleFraction( pvt::PHASE_TYPE::LIQUID_WATER_RICH, fraction );
}

}
