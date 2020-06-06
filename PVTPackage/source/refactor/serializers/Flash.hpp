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

#ifndef PVTPACKAGE_SERIALIZERS_FLASH_HPP
#define PVTPACKAGE_SERIALIZERS_FLASH_HPP

#include "MultiphaseSystem/PhaseSplitModel/BlackOilFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/DeadOilFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{

void to_json( nlohmann::json & j,
              const BlackOilFlash & flash );

void to_json( nlohmann::json & j,
              const DeadOilFlash & flash );

void to_json( nlohmann::json & j,
              const FreeWaterFlash & flash );

void to_json( nlohmann::json & j,
              const NegativeTwoPhaseFlash & flash );

void to_json( nlohmann::json & j,
              const TrivialFlash & flash );

}

#endif //PVTPACKAGE_SERIALIZERS_FLASH_HPP
