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

#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_Utils.hpp"
#include "Utils/math.hpp"

#include <algorithm>

namespace PVTPackage
{

void BlackOilUtils::refineTable( std::vector< std::vector< std::vector< double > > > & table,
                                 std::size_t nLevel )
{

  std::vector< double > refine_x;
  for( std::size_t i = 0; i < table.size(); ++i )
  {
    auto x_sat = table[i][0][0];
    for( std::size_t j = 0; j < table[i][0].size(); ++j )
    {
      refine_x.push_back( table[i][0][j] - x_sat );
    }
  }
  std::sort( refine_x.begin(), refine_x.end() );
  refine_x.erase( std::unique( refine_x.begin(), refine_x.end() ), refine_x.end() );

  //Linspace - This is optional and can be removed
  double maxval = *std::max_element( refine_x.begin(), refine_x.end() );
  refine_x = math::linspace( 0.0, maxval, nLevel );

  for( std::size_t i = 0; i < table.size(); ++i )
  {
    auto x_sat = table[i][0][0];
    for( std::size_t j = 0; j < table[i][0].size(); ++j )
    {
      table[i][0][j] = table[i][0][j] - x_sat;
    }

    for( std::size_t j = 1; j < table[i].size(); ++j )
    {
      auto prop_refined = math::Interpolation1( table[i][0], table[i][j], refine_x );
      table[i][j].resize( refine_x.size() );
      table[i][j] = prop_refined;
    }

    for( std::size_t j = 0; j < refine_x.size(); ++j )
    {
      table[i][0].resize( refine_x.size() );
      table[i][0][j] = refine_x[j] + x_sat;
    }
  }
}

}
