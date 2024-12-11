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

#include "./CompositionalApiInputs.hpp"

#include "./PVTEnums.hpp"
#include "../JsonKeys.hpp"

namespace PVTPackage
{
namespace tests
{
namespace pds
{

void from_json( const nlohmann::json & j,
                CompositionalApiInputs & output )
{
  j.at( FlashKeys::TYPE ).get_to( output.flashType );
  j.at( PublicAPIKeys::Compositional::PHASES ).get_to( output.phases );
  j.at( PublicAPIKeys::Compositional::EOS_TYPES ).get_to( output.eosTypes );
  j.at( PublicAPIKeys::Compositional::COMPONENT_NAMES ).get_to( output.componentNames );
  j.at( PublicAPIKeys::Compositional::COMPONENT_MOLAR_WEIGHTS ).get_to( output.componentMolarWeights );
  j.at( PublicAPIKeys::Compositional::COMPONENT_CRITICAL_TEMPERATURES ).get_to( output.componentCriticalTemperatures );
  j.at( PublicAPIKeys::Compositional::COMPONENT_CRITICAL_PRESSURES ).get_to( output.componentCriticalPressures );
  j.at( PublicAPIKeys::Compositional::COMPONENT_OMEGAS ).get_to( output.componentOmegas );
}

}
}
}