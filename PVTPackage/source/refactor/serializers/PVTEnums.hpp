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

#ifndef PVTPACKAGE_ENUMS_HPP
#define PVTPACKAGE_ENUMS_HPP

#include "refactor/JsonKeys.hpp"

#include "MultiphaseSystem/PVTEnums.hpp"

#include "pvt/pvt.hpp"

#include <nlohmann/json.hpp>

namespace pvt
{

NLOHMANN_JSON_SERIALIZE_ENUM( pvt::PHASE_TYPE, {
  { PHASE_TYPE::UNKNOWN, PVTPackage::refactor::EnumKeys::PHASE_TYPE_UNKNOWN },
  { PHASE_TYPE::LIQUID_WATER_RICH, PVTPackage::refactor::EnumKeys::PHASE_TYPE_LIQUID_WATER_RICH },
  { PHASE_TYPE::OIL, PVTPackage::refactor::EnumKeys::PHASE_TYPE_OIL },
  { PHASE_TYPE::GAS, PVTPackage::refactor::EnumKeys::PHASE_TYPE_GAS }
} )

NLOHMANN_JSON_SERIALIZE_ENUM( EOS_TYPE, {
  { EOS_TYPE::UNKNOWN, PVTPackage::refactor::EnumKeys::EOS_TYPE_UNKNOWN },
  { EOS_TYPE::REDLICH_KWONG_SOAVE, PVTPackage::refactor::EnumKeys::EOS_TYPE_REDLICH_KWONG_SOAVE },
  { EOS_TYPE::PENG_ROBINSON, PVTPackage::refactor::EnumKeys::EOS_TYPE_PENG_ROBINSON }
} )

}

namespace PVTPackage
{

NLOHMANN_JSON_SERIALIZE_ENUM( PHASE_STATE, {
  { PHASE_STATE::UNKNOWN, refactor::EnumKeys::PHASE_STATE_UNKNOWN },
  { PHASE_STATE::GAS, refactor::EnumKeys::PHASE_STATE_GAS },
  { PHASE_STATE::OIL, refactor::EnumKeys::PHASE_STATE_OIL },
  { PHASE_STATE::WATER, refactor::EnumKeys::PHASE_STATE_WATER },
  { PHASE_STATE::OIL_GAS, refactor::EnumKeys::PHASE_STATE_OIL_GAS },
  { PHASE_STATE::GAS_WATER, refactor::EnumKeys::PHASE_STATE_GAS_WATER },
  { PHASE_STATE::OIL_WATER, refactor::EnumKeys::PHASE_STATE_OIL_WATER },
  { PHASE_STATE::OIL_GAS_WATER, refactor::EnumKeys::PHASE_STATE_OIL_GAS_WATER }
} )

enum class PHASE_MODEL_TYPE : int
{
  CUBE_EOS, BLACK_OIL_GAS, BLACK_OIL_OIL, BLACK_OIL_WATER, DEAD_OIL
};

NLOHMANN_JSON_SERIALIZE_ENUM( PHASE_MODEL_TYPE, {
  { PHASE_MODEL_TYPE::CUBE_EOS, refactor::EnumKeys::PHASE_MODEL_TYPE_CUBE_EOS },
  { PHASE_MODEL_TYPE::BLACK_OIL_GAS, refactor::EnumKeys::PHASE_MODEL_TYPE_BLACK_OIL_GAS },
  { PHASE_MODEL_TYPE::BLACK_OIL_OIL, refactor::EnumKeys::PHASE_MODEL_TYPE_BLACK_OIL_OIL },
  { PHASE_MODEL_TYPE::BLACK_OIL_WATER, refactor::EnumKeys::PHASE_MODEL_TYPE_BLACK_OIL_WATER },
  { PHASE_MODEL_TYPE::DEAD_OIL, refactor::EnumKeys::PHASE_MODEL_TYPE_DEAD_OIL }
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

} // end of namespace PVTPackage

#endif //PVTPACKAGE_ENUMS_HPP
