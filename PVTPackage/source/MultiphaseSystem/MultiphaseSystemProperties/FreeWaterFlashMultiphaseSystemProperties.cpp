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

#include "FreeWaterFlashMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

FreeWaterFlashMultiphaseSystemProperties::FreeWaterFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                    std::size_t nComponents )
  :
  CompositionalMultiphaseSystemProperties( phases, nComponents )
{

}

void FreeWaterFlashMultiphaseSystemProperties::setWaterMoleComposition( std::vector< double > const & moleComposition )
{
  setMoleComposition( pvt::PHASE_TYPE::LIQUID_WATER_RICH, moleComposition );
}

void FreeWaterFlashMultiphaseSystemProperties::setWaterFraction( double const & fraction )
{
  setPhaseMoleFraction(pvt::PHASE_TYPE::LIQUID_WATER_RICH, fraction );
}

std::vector< double > const & FreeWaterFlashMultiphaseSystemProperties::getOilLnFugacity() const
{
  return m_lnFugacity.at( pvt::PHASE_TYPE::OIL );
}

std::vector< double > const & FreeWaterFlashMultiphaseSystemProperties::getGasLnFugacity() const
{
  return m_lnFugacity.at( pvt::PHASE_TYPE::GAS );
}

std::vector< double > const & FreeWaterFlashMultiphaseSystemProperties::getWaterLnFugacity() const
{
  return m_lnFugacity.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH );
}

}
