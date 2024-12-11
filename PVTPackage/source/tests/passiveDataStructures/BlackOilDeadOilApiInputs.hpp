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

#ifndef PVTPACKAGE_PDS_BLACKOILDEADOILAPIINPUTS_HPP
#define PVTPACKAGE_PDS_BLACKOILDEADOILAPIINPUTS_HPP

#include "./PVTEnums.hpp"

#include <string>
#include <vector>

namespace PVTPackage
{
namespace tests
{
namespace pds
{

struct BlackOilDeadOilApiInputs
{
public:

  typedef std::vector< std::vector< std::vector< double > > > TableDataType;

public:

  TableDataType tableData;
  std::vector< double > surfaceMassDensities;
  std::vector< pds::PHASE_TYPE > phases;
  std::vector< double > molecularWeights;

  bool operator==( BlackOilDeadOilApiInputs const & rhs ) const;

  bool operator!=( BlackOilDeadOilApiInputs const & rhs ) const;
};

}
}
}

#endif //PVTPACKAGE_PDS_BLACKOILDEADOILAPIINPUTS_HPP
