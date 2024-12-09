/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 TotalEnergies
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "../JsonKeys.hpp"

#include "./PropertyAndDerivatives.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage {
namespace tests {
namespace pds {

using json = nlohmann::json;

void from_json( const nlohmann::json & j,
                pds::ScalarPropertyAndDerivatives< double > & s )
{
  s.value = j[ScalarVectorPropertyAndDerivativesKeys::VALUE].get< double >();
  s.dP = j[ScalarVectorPropertyAndDerivativesKeys::DP].get< double >();
  s.dT = j[ScalarVectorPropertyAndDerivativesKeys::DT].get< double >();
  s.dz = j[ScalarVectorPropertyAndDerivativesKeys::DZ].get< std::vector< double > >();
}

void from_json( const nlohmann::json & j,
                pds::VectorPropertyAndDerivatives< double > & v )
{
  v.value = j[ScalarVectorPropertyAndDerivativesKeys::VALUE].get< std::vector< double > >();
  v.dP = j[ScalarVectorPropertyAndDerivativesKeys::DP].get< std::vector< double > >();
  v.dT = j[ScalarVectorPropertyAndDerivativesKeys::DT].get< std::vector< double > >();
  v.dz = j[ScalarVectorPropertyAndDerivativesKeys::DZ].get< std::vector< std::vector< double > > >();
}

} // end of namespace tests
} // end of namespace pds
} // end of namespace PVTPackage
