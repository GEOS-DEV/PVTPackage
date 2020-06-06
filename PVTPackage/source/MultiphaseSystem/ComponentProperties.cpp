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

#include "MultiphaseSystem/ComponentProperties.hpp"

#include "Utils/Assert.hpp"

namespace PVTPackage
{

ComponentProperties::ComponentProperties( const std::size_t & nComponents,
                                          const std::vector< std::string > & labels,
                                          const std::vector< double > & mw,
                                          const std::vector< double > & tc,
                                          const std::vector< double > & pc,
                                          const std::vector< double > & omega )
  : NComponents( nComponents ),
    Label( labels ),
    Mw( mw ),
    Pc( pc ),
    Tc( tc ),
    Omega( omega ),
    BIC( std::vector< std::vector< double > >( NComponents, std::vector< double >( NComponents, 0 ) ) ),
    VolumeShift( std::vector< std::vector< double > >( NComponents, std::vector< double >( 2, 0 ) ) ),
    WaterIndex( std::size_t( -1 ) ) // FIXME use a lambda and declare WaterIndex const.
{
  ASSERT( ( nComponents == labels.size() ) && ( nComponents == mw.size() ) && ( nComponents == tc.size() )
          && ( nComponents == pc.size() ) && ( nComponents == omega.size() ), "Dimension Mismatch." );
  for( std::size_t i = 0; i != NComponents; ++i )
  {
    if( Label[i] == "Water" || Label[i] == "water" || Label[i] == "H2O" || Label[i] == "h2o" )
    {
      WaterIndex = i;
    }
  }
}

bool ComponentProperties::operator==( const ComponentProperties & d ) const
{
  return
    NComponents == d.NComponents &&
    Label == d.Label &&
    Mw == d.Mw &&
    Pc == d.Pc &&
    Tc == d.Tc &&
    Omega == d.Omega &&
    BIC == d.BIC &&
    VolumeShift == d.VolumeShift &&
    WaterIndex == d.WaterIndex;
}

}
