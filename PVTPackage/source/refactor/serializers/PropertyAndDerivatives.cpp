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

#include "refactor/serializers/PropertyAndDerivatives.hpp"

#include "refactor/JsonKeys.hpp"

#include "pvt/pvt.hpp"

#include <nlohmann/json.hpp>

namespace pvt {

using json = nlohmann::json;

void to_json( json & j,
              const ScalarPropertyAndDerivatives< double > & s )
{
  using namespace PVTPackage::refactor;

  j = json{ { ScalarVectorPropertyAndDerivativesKeys::VALUE, s.value },
            { ScalarVectorPropertyAndDerivativesKeys::DP,    s.dP },
            { ScalarVectorPropertyAndDerivativesKeys::DT,    s.dT },
            { ScalarVectorPropertyAndDerivativesKeys::DZ,    s.dz } };
}

void to_json( json & j,
              const pvt::VectorPropertyAndDerivatives< double > & v )
{
  using namespace PVTPackage::refactor;

  j = json{ { ScalarVectorPropertyAndDerivativesKeys::VALUE, v.value },
            { ScalarVectorPropertyAndDerivativesKeys::DP,    v.dP },
            { ScalarVectorPropertyAndDerivativesKeys::DT,    v.dT },
            { ScalarVectorPropertyAndDerivativesKeys::DZ,    v.dz } };
}

} // end of namespace pvt