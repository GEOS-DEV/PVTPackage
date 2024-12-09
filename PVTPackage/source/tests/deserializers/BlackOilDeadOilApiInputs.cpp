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

#include "./BlackOilDeadOilApiInputs.hpp"

#include "../JsonKeys.hpp"

namespace PVTPackage
{
namespace tests
{
namespace pds
{

void from_json( const nlohmann::json & j,
                BlackOilDeadOilApiInputs & output )
{
  j.at( PublicAPIKeys::BlackOilDeadOil::PHASES ).get_to( output.phases );
  j.at( PublicAPIKeys::BlackOilDeadOil::TABLE_CONTENTS ).get_to( output.tableData );
  j.at( PublicAPIKeys::BlackOilDeadOil::SURFACE_MASS_DENSITIES ).get_to( output.surfaceMassDensities );
  j.at( PublicAPIKeys::BlackOilDeadOil::SURFACE_MOLECULAR_WEIGHTS ).get_to( output.molecularWeights );
}

}
}
}
