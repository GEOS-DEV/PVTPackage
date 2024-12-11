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

#include "CompositionalApiInputs.hpp"

namespace PVTPackage
{
namespace tests
{
namespace pds
{

bool CompositionalApiInputs::operator==( CompositionalApiInputs const & rhs ) const
{
  return flashType == rhs.flashType &&
         phases == rhs.phases &&
         eosTypes == rhs.eosTypes &&
         componentNames == rhs.componentNames &&
         componentMolarWeights == rhs.componentMolarWeights &&
         componentCriticalTemperatures == rhs.componentCriticalTemperatures &&
         componentCriticalPressures == rhs.componentCriticalPressures &&
         componentOmegas == rhs.componentOmegas;
}

bool CompositionalApiInputs::operator!=( CompositionalApiInputs const & rhs ) const
{
  return !( rhs == *this );
}

}
}
}
