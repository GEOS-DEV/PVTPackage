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

#include "TrivialFlashMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

TrivialFlashMultiphaseSystemProperties::TrivialFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                                                const ComponentProperties & componentProperties )
  :
  CompositionalMultiphaseSystemProperties( phases, eosTypes, componentProperties )
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
