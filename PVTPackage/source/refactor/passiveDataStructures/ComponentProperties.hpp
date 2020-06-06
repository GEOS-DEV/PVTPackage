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
#ifndef PVTPACKAGE_PDS_COMPONENTPROPERTIES_HPP
#define PVTPACKAGE_PDS_COMPONENTPROPERTIES_HPP

#pragma once

#include  <vector>
#include  <string>

namespace PVTPackage {
namespace refactor {
namespace pds {

struct ComponentProperties
{
  std::size_t NComponents;
  std::vector< std::string > Label;
  std::vector< double > Mw;
  std::vector< double > Pc;
  std::vector< double > Tc;
  std::vector< double > Omega;
  std::vector< std::vector< double>> BIC;
  std::vector< std::vector< double>> VolumeShift;
  std::size_t WaterIndex;
};

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_PDS_COMPONENTPROPERTIES_HPP
