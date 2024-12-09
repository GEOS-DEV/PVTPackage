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

#ifndef PVTPACKAGE_PDS_COMPOSITIONALAPIINPUTS_HPP
#define PVTPACKAGE_PDS_COMPOSITIONALAPIINPUTS_HPP

#include "./PVTEnums.hpp"

#include <string>
#include <vector>

namespace PVTPackage
{
namespace tests
{
namespace pds
{

struct CompositionalApiInputs
{
  pds::FLASH_TYPE flashType;
  std::vector< pds::PHASE_TYPE > phases;
  std::vector< pds::EOS_TYPE > eosTypes;
  std::vector< std::string > componentNames;
  std::vector< double > componentMolarWeights;
  std::vector< double > componentCriticalTemperatures;
  std::vector< double > componentCriticalPressures;
  std::vector< double > componentOmegas;

  bool operator==( CompositionalApiInputs const & rhs ) const;

  bool operator!=( CompositionalApiInputs const & rhs ) const;
};

}
}
}

#endif //PVTPACKAGE_PDS_COMPOSITIONALAPIINPUTS_HPP
