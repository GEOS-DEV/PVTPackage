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

#include "MultiphaseSystem/NegativeTwoPhaseMultiphaseSystem.hpp"

#include "refactor/deserializers/CompositionalApiInputs.hpp"
#include "refactor/deserializers/MultiphaseSystemProperties.hpp"

#include "refactor/passiveDataStructures/CompositionalApiInputs.hpp"
#include "refactor/JsonKeys.hpp"

#include "TestFactor.hpp"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace PVTPackage
{
namespace refactor
{

using json = nlohmann::json;

void validateParallel( const std::string & fileName )
{
  // Build validation data
  std::vector< double > pressures, temperatures;
  std::vector< std::vector< double > > feeds;
  std::vector< pds::PDSMSP > refsMsp;

  pds::CompositionalApiInputs apiInputs;

  std::string line;
  std::ifstream dataFile( fileName );
  while( std::getline( dataFile, line ) )
  {
    const bool isComment = line.rfind( "#", 0 ) == 0;
    if( not line.empty() and not isComment )
    {
      const json j = json::parse( line );

//      if( apiInputs == nullptr )
      {
        // FIXME EVERYLINE! WRONG!
        j.at( PublicAPIKeys::INPUT ).at( PublicAPIKeys::API ).get_to( apiInputs );
      }

      const auto refMsp = j.at( PublicAPIKeys::OUTPUT ).get< pds::PDSMSP >();
      refsMsp.emplace_back( refMsp );

      const json & computation = j.at( PublicAPIKeys::INPUT ).at( PublicAPIKeys::COMPUTATION );
      const double pressure = computation.at( PublicAPIKeys::Computation::PRESSURE ).get< double >();
      const double temperature = computation.at( PublicAPIKeys::Computation::TEMPERATURE ).get< double >();
      const std::vector< double > feed = computation.at( PublicAPIKeys::Computation::FEED ).get< std::vector< double > >();
      pressures.emplace_back( pressure );
      temperatures.emplace_back( temperature );
      feeds.emplace_back( feed );
    }
  }

  auto systemPtr = NegativeTwoPhaseMultiphaseSystem::build(
    convert( apiInputs.phases ),
    convert( apiInputs.eosTypes ),
    apiInputs.componentNames,
    apiInputs.componentMolarWeights,
    apiInputs.componentCriticalTemperatures,
    apiInputs.componentCriticalPressures,
    apiInputs.componentOmegas
  );

  systemPtr->BatchUpdate( pressures, temperatures, feeds );

  const auto & solutions = systemPtr->getBatchMultiphaseSystemProperties();

  solutions.size();

  ASSERT_EQ( refsMsp.size(), solutions.size() );

  for( std::size_t i = 0; i < solutions.size(); ++i )
  {
    const NegativeTwoPhaseFlashMultiphaseSystemProperties & actual = solutions[i];
    const pds::PDSMSP & refMsp = refsMsp[i];

    compare( actual, refMsp );
  }
}

TEST( pvt, parallel )
{
//  validateParallel( "data/compositional_multiphase_wells_1d.log.err" );
  validateParallel( "data/compositional_multiphase_wells_2d.log.err" );
}

int main( int argc,
          char ** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  int const result = RUN_ALL_TESTS();

  return result;
}


}
}