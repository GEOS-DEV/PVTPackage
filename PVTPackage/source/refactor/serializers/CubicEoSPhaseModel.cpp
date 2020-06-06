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

#include "CubicEoSPhaseModel.hpp"

#include "refactor/JsonKeys.hpp"

#include "refactor/serializers/PVTEnums.hpp"
#include "refactor/serializers/ComponentProperties.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace PVTPackage
{

void to_json( json & j,
              const CubicEoSPhaseModel & model )
{
  using namespace PVTPackage::refactor;

  j = json{ { CubeEoSPhaseModelsKeys::EOS,                  model.getEosType() },
            { CubeEoSPhaseModelsKeys::PHASE_TYPE_,          model.getPhase() },
            { CubeEoSPhaseModelsKeys::COMPONENT_PROPERTIES, model.getComponentsProperties() } };
}

} // end of namespace PVTPackage
