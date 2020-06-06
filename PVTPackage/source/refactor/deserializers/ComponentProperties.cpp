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

#include "refactor/JsonKeys.hpp"

#include "refactor/deserializers/ComponentProperties.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

void from_json( const nlohmann::json & j, pds::ComponentProperties & p )
{
  j.at( ComponentPropertiesKeys::N_COMPONENTS ).get_to( p.NComponents );
  j.at( ComponentPropertiesKeys::LABELS ).get_to( p.Label );
  j.at( ComponentPropertiesKeys::MW ).get_to( p.Mw );
  j.at( ComponentPropertiesKeys::PC ).get_to( p.Pc );
  j.at( ComponentPropertiesKeys::TC ).get_to( p.Tc );
  j.at( ComponentPropertiesKeys::OMEGA ).get_to( p.Omega );
  j.at( ComponentPropertiesKeys::WATER_INDEX ).get_to( p.WaterIndex );}

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage
