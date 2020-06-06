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

#pragma once

#include <unordered_map>
#include <vector>

namespace PVTPackage
{

enum class PHASE_STATE : int
{
  GAS = 6, OIL = 5, WATER = 4, OIL_GAS = 3, GAS_WATER = 2, OIL_WATER = 1, OIL_GAS_WATER = 0, UNKNOWN = -1
};

const std::unordered_map< std::vector< bool >, PHASE_STATE > PhaseStateMap =
  {
    { { true,  true,  true },  PHASE_STATE::OIL_GAS_WATER },
    { { true,  false, true },  PHASE_STATE::OIL_WATER },
    { { true,  true,  false }, PHASE_STATE::OIL_GAS },
    { { false, true,  true },  PHASE_STATE::GAS_WATER },
    { { false, false, true },  PHASE_STATE::WATER },
    { { true,  false, false }, PHASE_STATE::OIL },
    { { false, true,  false }, PHASE_STATE::GAS },
  };

}
