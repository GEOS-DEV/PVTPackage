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

#include "refactor/JsonKeys.hpp"

#include "refactor/passiveDataStructures/PVTEnums.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

NLOHMANN_JSON_SERIALIZE_ENUM( pds::PHASE_STATE, {
  { pds::PHASE_STATE::UNKNOWN, EnumKeys::PHASE_STATE_UNKNOWN },
  { pds::PHASE_STATE::GAS, EnumKeys::PHASE_STATE_GAS },
  { pds::PHASE_STATE::OIL, EnumKeys::PHASE_STATE_OIL },
  { pds::PHASE_STATE::WATER, EnumKeys::PHASE_STATE_WATER },
  { pds::PHASE_STATE::OIL_GAS, EnumKeys::PHASE_STATE_OIL_GAS },
  { pds::PHASE_STATE::GAS_WATER, EnumKeys::PHASE_STATE_GAS_WATER },
  { pds::PHASE_STATE::OIL_WATER, EnumKeys::PHASE_STATE_OIL_WATER },
  { pds::PHASE_STATE::OIL_GAS_WATER, EnumKeys::PHASE_STATE_OIL_GAS_WATER }
} )

NLOHMANN_JSON_SERIALIZE_ENUM( pds::PHASE_TYPE, {
  { pds::PHASE_TYPE::UNKNOWN, EnumKeys::PHASE_TYPE_UNKNOWN },
  { pds::PHASE_TYPE::LIQUID_WATER_RICH, EnumKeys::PHASE_TYPE_LIQUID_WATER_RICH },
  { pds::PHASE_TYPE::OIL, EnumKeys::PHASE_TYPE_OIL },
  { pds::PHASE_TYPE::GAS, EnumKeys::PHASE_TYPE_GAS }
} )

NLOHMANN_JSON_SERIALIZE_ENUM( pds::EOS_TYPE, {
  { pds::EOS_TYPE::UNKNOWN, EnumKeys::EOS_TYPE_UNKNOWN },
  { pds::EOS_TYPE::REDLICH_KWONG_SOAVE, EnumKeys::EOS_TYPE_REDLICH_KWONG_SOAVE },
  { pds::EOS_TYPE::PENG_ROBINSON, EnumKeys::EOS_TYPE_PENG_ROBINSON }
} )

NLOHMANN_JSON_SERIALIZE_ENUM( pds::PHASE_MODEL_TYPE, {
  { pds::PHASE_MODEL_TYPE::CUBE_EOS, EnumKeys::PHASE_MODEL_TYPE_CUBE_EOS },
  { pds::PHASE_MODEL_TYPE::BLACK_OIL_GAS, EnumKeys::PHASE_MODEL_TYPE_BLACK_OIL_GAS },
  { pds::PHASE_MODEL_TYPE::BLACK_OIL_OIL, EnumKeys::PHASE_MODEL_TYPE_BLACK_OIL_OIL },
  { pds::PHASE_MODEL_TYPE::BLACK_OIL_WATER, EnumKeys::PHASE_MODEL_TYPE_BLACK_OIL_WATER },
  { pds::PHASE_MODEL_TYPE::DEAD_OIL, EnumKeys::PHASE_MODEL_TYPE_DEAD_OIL }
} )

enum class FLASH_TYPE : int
{
  BLACK_OIL, DEAD_OIL, FREE_WATER, NEGATIVE_TWO_PHASE, TRIVIAL
};

NLOHMANN_JSON_SERIALIZE_ENUM( FLASH_TYPE, {
  { FLASH_TYPE::BLACK_OIL, refactor::FlashKeys::BLACK_OIL },
  { FLASH_TYPE::DEAD_OIL, refactor::FlashKeys::DEAD_OIL },
  { FLASH_TYPE::FREE_WATER, refactor::FlashKeys::FREE_WATER },
  { FLASH_TYPE::NEGATIVE_TWO_PHASE, refactor::FlashKeys::NEGATIVE_TWO_PHASE },
  { FLASH_TYPE::TRIVIAL, refactor::FlashKeys::TRIVIAL }
} )

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_DESERIALIZERS_PVTENUMS_HPP
