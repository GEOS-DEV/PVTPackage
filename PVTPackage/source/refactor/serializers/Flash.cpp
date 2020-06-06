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

#include "MultiphaseSystem/PhaseSplitModel/BlackOilFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/DeadOilFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"

#include "refactor/serializers/PVTEnums.hpp"
#include "refactor/serializers/ComponentProperties.hpp"
#include "refactor/JsonKeys.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{

using json = nlohmann::json;

void to_json( nlohmann::json & j,
              const BlackOilFlash & flash )
{
  (void) flash;
  j = json{};
}

void to_json( nlohmann::json & j,
              const DeadOilFlash & flash )
{
  (void) flash;
  j = json{};
}

void to_json( nlohmann::json & j,
              const FreeWaterFlash & flash )
{
  j = json{
    { refactor::CompositionalFlashKeys::COMPONENT_PROPERTIES, flash.getComponentProperties() },
    { refactor::FreeWaterFlashKeys::WATER_INDEX,              flash.getWaterIndex() }
  };
}

void to_json( nlohmann::json & j,
              const NegativeTwoPhaseFlash & flash )
{
  j = json{
    { refactor::CompositionalFlashKeys::COMPONENT_PROPERTIES, flash.getComponentProperties() }
  };
}

void to_json( nlohmann::json & j,
              const TrivialFlash & flash )
{
  j = json{
    { refactor::CompositionalFlashKeys::COMPONENT_PROPERTIES, flash.getComponentProperties() }
  };
}

}