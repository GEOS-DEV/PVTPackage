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

#ifndef GEOSX_REFACTOR_DESERIALIZERS_FLASH_HPP
#define GEOSX_REFACTOR_DESERIALIZERS_FLASH_HPP

#include "refactor/passiveDataStructures/Flash.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

void from_json( const nlohmann::json & j,
                pds::BlackOilFlash & flash );

void from_json( const nlohmann::json & j,
                pds::DeadOilFlash & flash );

void from_json( const nlohmann::json & j,
                pds::FreeWaterFlash & flash );

void from_json( const nlohmann::json & j,
                pds::NegativeTwoPhaseFlash & flash );

void from_json( const nlohmann::json & j,
                pds::TrivialFlash & flash );


} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage



#endif //GEOSX_REFACTOR_DESERIALIZERS_FLASH_HPP
