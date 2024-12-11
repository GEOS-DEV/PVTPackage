/*	
 * ------------------------------------------------------------------------------------------------------------	
 * SPDX-License-Identifier: LGPL-2.1-only	
 *	
 * Copyright (c) 2018-2024 Lawrence Livermore National Security LLC	
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University	
 * Copyright (c) 2018-2024 TotalEnergies	
 * Copyright (c) 2019-     GEOS/GEOSX Contributors	
 * All right reserved	
 *	
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.	
 * ------------------------------------------------------------------------------------------------------------	
 */

#ifndef PVTPACKAGE_DESERIALIZERS_COMPOSITIONALAPIINPUTS_HPP
#define PVTPACKAGE_DESERIALIZERS_COMPOSITIONALAPIINPUTS_HPP

#include "../passiveDataStructures/CompositionalApiInputs.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{
namespace tests
{
namespace pds
{

void from_json( const nlohmann::json & j,
                CompositionalApiInputs & output );

}
}
}

#endif //PVTPACKAGE_DESERIALIZERS_COMPOSITIONALAPIINPUTS_HPP
