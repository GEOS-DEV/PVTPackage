/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 Total, S.A
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#ifndef PVTPACKAGE_COMPONENTPROPERTIES_HPP
#define PVTPACKAGE_COMPONENTPROPERTIES_HPP

#include <string>
#include <vector>

namespace PVTPackage
{

struct ComponentProperties
{
  ComponentProperties( const std::size_t & nComponents,
                       const std::vector< std::string > & labels,
                       const std::vector< double > & mw,
                       const std::vector< double > & tc,
                       const std::vector< double > & pc,
                       const std::vector< double > & omega );

  bool operator==( const ComponentProperties & d ) const;

  const unsigned long NComponents;
  const std::vector< std::string > Label;
  const std::vector< double > Mw;
  const std::vector< double > Pc;
  const std::vector< double > Tc;
  const std::vector< double > Omega;
  const std::vector< std::vector< double > > BIC;
  const std::vector< std::vector< double > > VolumeShift;
  unsigned long WaterIndex;
};

}

#endif //PVTPACKAGE_COMPONENTPROPERTIES_HPP
