/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include <unordered_map>
#include <vector>

namespace PVTPackage
{

enum class PVT_TYPE : int
{ BLACK_OIL = 0, DEAD = 0, GENERAL = 1, MODIFIED = 2, API_TRACKING = 3, COMPOSITIONAL = 4, UNKNOWN = -1};

enum class COMPOSITIONAL_FLASH_TYPE : int
{ TRIVIAL = 0, NEGATIVE_OIL_GAS = 1, TABULATED_KVALUES = 2, FREE_WATER = 3, THREE_PHASE = 4, UNKNOWN = -1 };

enum class EOS_TYPE : int
{ REDLICH_KWONG_SOAVE = 1, PENG_ROBINSON = 0, UNKNOWN = -1 };

enum class PHASE_TYPE : int
{ LIQUID_WATER_RICH = 2, OIL = 1, GAS = 0, UNKNOWN = -1 };

enum class PHASE_STATE :int
{ GAS = 6, OIL = 5, WATER = 4, OIL_GAS = 3, GAS_WATER = 2, OIL_WATER = 1, OIL_GAS_WATER = 0, UNKNOWN = -1 };

enum class STATE_VARIABLE : int
{TEMPERATURE,PRESSURE,FEED};


struct EnumClassHash
{
  template <typename T>
  std::size_t operator()(T t) const
  {
    return static_cast<int>(t);
  }
};


//Phase State
const std::unordered_map<std::vector<bool>, PHASE_STATE> PhaseStateMap =
  {
    { { 1,1,1 } ,PHASE_STATE::OIL_GAS_WATER },
    { { 1,0,1 } ,PHASE_STATE::OIL_WATER },
    { { 1,1,0 } ,PHASE_STATE::OIL_GAS },
    { { 0,1,1 } ,PHASE_STATE::GAS_WATER },
    { { 0,0,1 } ,PHASE_STATE::WATER },
    { { 1,0,0 } ,PHASE_STATE::OIL },
    { { 0,1,0 } ,PHASE_STATE::GAS },
  };

}
