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

#include "NegativeTwoPhaseFlashMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

NegativeTwoPhaseFlashMultiphaseSystemProperties::NegativeTwoPhaseFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                                  std::size_t nComponents )
  :
  CompositionalMultiphaseSystemProperties( phases, nComponents )
{

}

std::vector< double > const & NegativeTwoPhaseFlashMultiphaseSystemProperties::getOilLnFugacity() const
{
  return m_lnFugacity.at( pvt::PHASE_TYPE::OIL );
}

std::vector< double > const & NegativeTwoPhaseFlashMultiphaseSystemProperties::getGasLnFugacity() const
{
  return m_lnFugacity.at( pvt::PHASE_TYPE::GAS );
}

}
