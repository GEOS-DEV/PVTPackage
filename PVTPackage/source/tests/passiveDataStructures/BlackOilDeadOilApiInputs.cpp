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

#include "BlackOilDeadOilApiInputs.hpp"

namespace PVTPackage
{
namespace tests
{
namespace pds
{

bool BlackOilDeadOilApiInputs::operator==( BlackOilDeadOilApiInputs const & rhs ) const
{
  return phases == rhs.phases &&
         tableData == rhs.tableData &&
         surfaceMassDensities == rhs.surfaceMassDensities &&
         molecularWeights == rhs.molecularWeights;
}

bool BlackOilDeadOilApiInputs::operator!=( BlackOilDeadOilApiInputs const & rhs ) const
{
  return !( rhs == *this );
}

}
}
}
