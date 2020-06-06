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

#include "Flash.hpp"

#include "refactor/deserializers/ComponentProperties.hpp"

#include "refactor/JsonKeys.hpp"

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

void from_json( const nlohmann::json & j,
                pds::BlackOilFlash & flash )
{
  (void) j;
  (void) flash;
}

void from_json( const nlohmann::json & j,
                pds::DeadOilFlash & flash )
{
  (void) j;
  (void) flash;
}

void from_json( const nlohmann::json & j,
                pds::CompositionalFlash & flash )
{
  j.at( CompositionalFlashKeys::COMPONENT_PROPERTIES ).get_to( flash.componentsProperties );
}

void from_json( const nlohmann::json & j,
                pds::FreeWaterFlash & flash )
{
  pds::CompositionalFlash & comp = static_cast<pds::CompositionalFlash &>(flash);
  from_json( j, comp );

  j.at( FreeWaterFlashKeys::WATER_INDEX ).get_to( flash.waterIndex );
}

void from_json( const nlohmann::json & j,
                pds::NegativeTwoPhaseFlash & flash )
{
  pds::CompositionalFlash & comp = static_cast<pds::CompositionalFlash &>(flash);
  from_json( j, comp );
}

void from_json( const nlohmann::json & j,
                pds::TrivialFlash & flash )
{
  pds::CompositionalFlash & comp = static_cast<pds::CompositionalFlash &>(flash);
  from_json( j, comp );
}

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

