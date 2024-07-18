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

#include "pvt/pvt.hpp"

// FIXME The registration is wrong so we have to depend on implementations
#include "MultiphaseSystem/TrivialMultiphaseSystem.hpp"
#include "MultiphaseSystem/NegativeTwoPhaseMultiphaseSystem.hpp"
#include "MultiphaseSystem/FreeWaterMultiphaseSystem.hpp"
#include "MultiphaseSystem/BlackOilMultiphaseSystem.hpp"
#include "MultiphaseSystem/DeadOilMultiphaseSystem.hpp"

#include <functional>
#include <map>

namespace pvt
{

std::unique_ptr< MultiphaseSystem > MultiphaseSystemBuilder::buildCompositional( COMPOSITIONAL_FLASH_TYPE const & flashType,
                                                                                 std::vector< PHASE_TYPE > const & phases,
                                                                                 std::vector< EOS_TYPE > const & eosTypes,
                                                                                 std::vector< std::string > const & componentNames,
                                                                                 std::vector< double > const & componentMolarWeights,
                                                                                 std::vector< double > const & componentCriticalTemperatures,
                                                                                 std::vector< double > const & componentCriticalPressures,
                                                                                 std::vector< double > const & componentOmegas )
{
  typedef std::function< std::unique_ptr< MultiphaseSystem >(
    const std::vector< pvt::PHASE_TYPE > &,
    const std::vector< pvt::EOS_TYPE > &,
    const std::vector< std::string > &,
    const std::vector< double > &,
    const std::vector< double > &,
    const std::vector< double > &,
    const std::vector< double > &
  ) > builderType;

  // FIXME The registration is wrong so we have to depend on implementations
  const std::map< pvt::COMPOSITIONAL_FLASH_TYPE, builderType > m{
    { pvt::COMPOSITIONAL_FLASH_TYPE::TRIVIAL,          PVTPackage::TrivialMultiphaseSystem::build },
    { pvt::COMPOSITIONAL_FLASH_TYPE::NEGATIVE_OIL_GAS, PVTPackage::NegativeTwoPhaseMultiphaseSystem::build },
    { pvt::COMPOSITIONAL_FLASH_TYPE::FREE_WATER,       PVTPackage::FreeWaterMultiphaseSystem::build }
  };

  try
  {
    builderType const & builder = m.at( flashType );
    return builder( phases,
                    eosTypes,
                    componentNames,
                    componentMolarWeights,
                    componentCriticalTemperatures,
                    componentCriticalPressures,
                    componentOmegas );
  }
  catch( const std::out_of_range & e )
  {
    std::string msg = "Could not find error messages for flash type " + std::to_string( static_cast< int >( flashType ) );
    LOGERROR( msg );
    return std::unique_ptr< MultiphaseSystem >();
  }
}

std::unique_ptr< MultiphaseSystem > MultiphaseSystemBuilder::buildLiveOil( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                           const std::vector< std::string > & tableFileNames,
                                                                           const std::vector< double > & surfaceMassDensities,
                                                                           const std::vector< double > & molarWeights )
{
  return PVTPackage::BlackOilMultiphaseSystem::build( phases, tableFileNames, surfaceMassDensities, molarWeights );
}

std::unique_ptr< MultiphaseSystem > MultiphaseSystemBuilder::buildDeadOil( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                           const std::vector< std::string > & tableFileNames,
                                                                           const std::vector< double > & surfaceMassDensities,
                                                                           const std::vector< double > & molarWeights )
{
  return PVTPackage::DeadOilMultiphaseSystem::build( phases, tableFileNames, surfaceMassDensities, molarWeights );
}

}