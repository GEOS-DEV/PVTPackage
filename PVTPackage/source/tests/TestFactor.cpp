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

#include "TestFactor.hpp"

#include <algorithm>

namespace PVTPackage
{
namespace tests
{

pvt::COMPOSITIONAL_FLASH_TYPE convert( const pds::FLASH_TYPE & input )
{
  const std::map< pds::FLASH_TYPE, pvt::COMPOSITIONAL_FLASH_TYPE > m{
    { pds::FLASH_TYPE::NEGATIVE_TWO_PHASE, pvt::COMPOSITIONAL_FLASH_TYPE::NEGATIVE_OIL_GAS },
    { pds::FLASH_TYPE::FREE_WATER,         pvt::COMPOSITIONAL_FLASH_TYPE::FREE_WATER },
    { pds::FLASH_TYPE::TRIVIAL,            pvt::COMPOSITIONAL_FLASH_TYPE::TRIVIAL }
  };

  return m.at( input );
}

pvt::PHASE_TYPE convert( const pds::PHASE_TYPE & input )
{
  const std::map< pds::PHASE_TYPE, pvt::PHASE_TYPE > m{
    { pds::PHASE_TYPE::LIQUID_WATER_RICH, pvt::PHASE_TYPE::LIQUID_WATER_RICH },
    { pds::PHASE_TYPE::OIL,               pvt::PHASE_TYPE::OIL },
    { pds::PHASE_TYPE::GAS,               pvt::PHASE_TYPE::GAS },
    { pds::PHASE_TYPE::UNKNOWN,           pvt::PHASE_TYPE::UNKNOWN }
  };

  return m.at( input );
}

pvt::EOS_TYPE convert( const pds::EOS_TYPE & input )
{
  const std::map< pds::EOS_TYPE, pvt::EOS_TYPE > m{
    { pds::EOS_TYPE::REDLICH_KWONG_SOAVE, pvt::EOS_TYPE::REDLICH_KWONG_SOAVE },
    { pds::EOS_TYPE::PENG_ROBINSON,       pvt::EOS_TYPE::PENG_ROBINSON },
    { pds::EOS_TYPE::UNKNOWN,             pvt::EOS_TYPE::UNKNOWN }
  };

  return m.at( input );
}


std::vector< pvt::PHASE_TYPE > convert( const std::vector< pds::PHASE_TYPE > & input )
{
  // FIXME I cannot give raw function to std::transform?
  auto f = []( const pds::PHASE_TYPE & i )
  {
    return convert( i );
  };

  std::vector< pvt::PHASE_TYPE > output( input.size() );
  std::transform( input.cbegin(), input.cend(), output.begin(), f );

  return output;
}

std::vector< pvt::EOS_TYPE > convert( const std::vector< pds::EOS_TYPE > & input )
{
  // FIXME I cannot give raw function to std::transform?
  auto f = []( const pds::EOS_TYPE & i )
  {
    return convert( i );
  };

  std::vector< pvt::EOS_TYPE > output( input.size() );
  std::transform( input.cbegin(), input.cend(), output.begin(), f );

  return output;
}

void compare( const pvt::MultiphaseSystemProperties & actual,
              const pds::PDSMSP & expected )
{
  const std::set< pds::PHASE_TYPE > phases = expected.getPhases();
  for( const auto & pdsPhase: phases )
  {
    pvt::PHASE_TYPE phase = convert( pdsPhase );

    compare( actual.getMassDensity( phase ), expected.getMassDensity( pdsPhase ), 1.e-10 );
    compare( actual.getMoleComposition( phase ), expected.getMoleComposition( pdsPhase ), 1.e-10 );
    compare( actual.getMoleDensity( phase ), expected.getMoleDensity( pdsPhase ), 1.e-10 );
    compare( actual.getMolecularWeight( phase ), expected.getMolecularWeight( pdsPhase ), 1.e-10 );
    compare( actual.getPhaseMoleFraction( phase ), expected.getPhaseMoleFraction( pdsPhase ), 1.e-10 );
    compare( actual.getViscosity( phase ), expected.getViscosity( pdsPhase ), 1.e-10 );
  }
}

}
}
