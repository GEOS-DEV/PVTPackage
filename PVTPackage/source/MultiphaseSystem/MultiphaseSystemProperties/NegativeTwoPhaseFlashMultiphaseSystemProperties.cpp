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

#include "NegativeTwoPhaseFlashMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

NegativeTwoPhaseFlashMultiphaseSystemProperties::NegativeTwoPhaseFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                                  const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                                                                  const ComponentProperties & cp )
  :
  CompositionalMultiphaseSystemProperties( phases, eosTypes, cp )
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
