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

#include "refactor/serializers/ComponentProperties.hpp"

#include "refactor/JsonKeys.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage {

using json = nlohmann::json;

void to_json( nlohmann::json & j,
              const ComponentProperties & p )
{
  using namespace PVTPackage::refactor;

  j = json{ { ComponentPropertiesKeys::N_COMPONENTS, p.NComponents },
            { ComponentPropertiesKeys::LABELS,       p.Label },
            { ComponentPropertiesKeys::MW,           p.Mw },
            { ComponentPropertiesKeys::TC,           p.Tc },
            { ComponentPropertiesKeys::PC,           p.Pc },
            { ComponentPropertiesKeys::OMEGA,        p.Omega },
            { ComponentPropertiesKeys::WATER_INDEX,  p.WaterIndex } };
}

} // namespace PVTPackage
