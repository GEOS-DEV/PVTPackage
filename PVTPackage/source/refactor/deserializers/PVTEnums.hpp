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

#ifndef PVTPACKAGE_DESERIALIZERS_PVTENUMS_HPP
#define PVTPACKAGE_DESERIALIZERS_PVTENUMS_HPP

#include "../JsonKeys.hpp"

#include "../passiveDataStructures/PVTEnums.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

NLOHMANN_JSON_SERIALIZE_ENUM( pds::PHASE_TYPE, {
  { pds::PHASE_TYPE::UNKNOWN, PhaseTypeKeys::UNKNOWN },
  { pds::PHASE_TYPE::LIQUID_WATER_RICH, PhaseTypeKeys::LIQUID_WATER_RICH },
  { pds::PHASE_TYPE::OIL, PhaseTypeKeys::OIL },
  { pds::PHASE_TYPE::GAS, PhaseTypeKeys::GAS }
} )

NLOHMANN_JSON_SERIALIZE_ENUM( pds::EOS_TYPE, {
  { pds::EOS_TYPE::UNKNOWN, EnumKeys::EOS_TYPE_UNKNOWN },
  { pds::EOS_TYPE::REDLICH_KWONG_SOAVE, EnumKeys::EOS_TYPE_REDLICH_KWONG_SOAVE },
  { pds::EOS_TYPE::PENG_ROBINSON, EnumKeys::EOS_TYPE_PENG_ROBINSON }
} )

NLOHMANN_JSON_SERIALIZE_ENUM( pds::FLASH_TYPE, {
  { pds::FLASH_TYPE::BLACK_OIL, FlashKeys::BLACK_OIL },
  { pds::FLASH_TYPE::DEAD_OIL, FlashKeys::DEAD_OIL },
  { pds::FLASH_TYPE::FREE_WATER, FlashKeys::FREE_WATER },
  { pds::FLASH_TYPE::NEGATIVE_TWO_PHASE, FlashKeys::NEGATIVE_TWO_PHASE },
  { pds::FLASH_TYPE::TRIVIAL, FlashKeys::TRIVIAL }
} )

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_DESERIALIZERS_PVTENUMS_HPP
