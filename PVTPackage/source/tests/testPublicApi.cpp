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

#include "./deserializers/PVTEnums.hpp"
#include "./deserializers/BlackOilDeadOilApiInputs.hpp"
#include "./deserializers/CompositionalApiInputs.hpp"
#include "./deserializers/MultiphaseSystemProperties.hpp"

#include "./serializers/MultiphaseSystemProperties.hpp"

#include "./JsonKeys.hpp"

#include "./passiveDataStructures/BlackOilDeadOilApiInputs.hpp"
#include "./passiveDataStructures/CompositionalApiInputs.hpp"
#include "./passiveDataStructures/MultiphaseSystemProperties.hpp"

#include "./TestFactor.hpp"

#include "pvt/pvt.hpp"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <vector>

namespace PVTPackage
{
namespace tests
{

using json = nlohmann::json;

// FIXME Use tests setup
class Holder
{
private:

  // FIXME Do not forget to clean
  std::vector< std::string > tableFileNames;

  static std::vector< std::string > buildTableFileNames( const pds::BlackOilDeadOilApiInputs::TableDataType & tableData )
  {
    std::vector< std::string > tableFileNames;
    for( const std::vector< std::vector< double > > & matrix: tableData )
    {
      std::string outputFile = std::tmpnam( nullptr );
//      std::cerr << outputFile << std::endl;
      std::ofstream ofs( outputFile );
      ofs << std::setprecision( std::numeric_limits< long double >::digits10 + 1 );

      for( const std::vector< double > & line: matrix )
      {
        for( const double & value: line )
        {
          ofs << value << " ";
        }
        ofs << std::endl;
      }

      tableFileNames.push_back( outputFile );
    }
    return tableFileNames;
  }

  // FIXME unique_ptr??? who holds?
  typedef std::pair< pds::CompositionalApiInputs, std::unique_ptr< pvt::MultiphaseSystem > > compInput2system;
  typedef std::pair< pds::BlackOilDeadOilApiInputs, std::unique_ptr< pvt::MultiphaseSystem > > blackOilDeadOilInput2system;
  std::list< compInput2system > compositionalSystems;
  std::list< blackOilDeadOilInput2system > blackOilSystems;
  std::list< blackOilDeadOilInput2system > deadOilSystems;

  // FIXME Return a ref??
  pvt::MultiphaseSystem * getBlackOilSystem( const pds::BlackOilDeadOilApiInputs & apiBuildParams )
  {
    auto predicate = [&apiBuildParams]( const blackOilDeadOilInput2system & pair )
    {
      return pair.first == apiBuildParams;
    };

    auto iter = std::find_if( blackOilSystems.begin(), blackOilSystems.end(), predicate );

    if( iter != blackOilSystems.end() )
    {
      return iter->second.get();
    }
    else
    {
      tableFileNames = buildTableFileNames( apiBuildParams.tableData );
      auto pair = std::make_pair( apiBuildParams,
                                  pvt::MultiphaseSystemBuilder::buildLiveOil(
                                    convert( apiBuildParams.phases ),
                                    tableFileNames,
                                    apiBuildParams.surfaceMassDensities,
                                    apiBuildParams.molecularWeights
                                  ) );
      blackOilSystems.push_back( std::move( pair ) );
      return blackOilSystems.back().second.get();
    }
  }

  pvt::MultiphaseSystem * getDeadOilSystem( const pds::BlackOilDeadOilApiInputs & apiBuildParams )
  {
    auto predicate = [&apiBuildParams]( const blackOilDeadOilInput2system & pair )
    {
      return pair.first == apiBuildParams;
    };

    auto iter = std::find_if( deadOilSystems.begin(), deadOilSystems.end(), predicate );

    if( iter != deadOilSystems.end() )
    {
      return iter->second.get();
    }
    else
    {
      tableFileNames = buildTableFileNames( apiBuildParams.tableData );
      auto pair = std::make_pair( apiBuildParams,
                                  pvt::MultiphaseSystemBuilder::buildDeadOil(
                                    convert( apiBuildParams.phases ),
                                    tableFileNames,
                                    apiBuildParams.surfaceMassDensities,
                                    apiBuildParams.molecularWeights
                                  ) );
      deadOilSystems.push_back( std::move( pair ) );
      return deadOilSystems.back().second.get();
    }
  }

public:

  pvt::MultiphaseSystem * getDeadOilBlackOilSystem( pds::FLASH_TYPE flashType,
                                                    const pds::BlackOilDeadOilApiInputs & apiBuildParams )
  {
    if( flashType == pds::FLASH_TYPE::BLACK_OIL )
    {
      return this->getBlackOilSystem( apiBuildParams );
    }
    else if( flashType == pds::FLASH_TYPE::DEAD_OIL )
    {
      return this->getDeadOilSystem( apiBuildParams );
    }
    else
    {
      return nullptr;
    }
  }

  pvt::MultiphaseSystem * getCompositionalSystem( const pds::CompositionalApiInputs & apiBuildParams )
  {
    auto predicate = [&apiBuildParams]( const compInput2system & pair )
    {
      return pair.first == apiBuildParams;
    };

    auto iter = std::find_if( compositionalSystems.begin(), compositionalSystems.end(), predicate );

    if( iter != compositionalSystems.end() )
    {
      return iter->second.get();
    }
    else
    {
      auto pair = std::make_pair( apiBuildParams,
                                  pvt::MultiphaseSystemBuilder::buildCompositional(
                                    convert( apiBuildParams.flashType ),
                                    convert( apiBuildParams.phases ),
                                    convert( apiBuildParams.eosTypes ),
                                    apiBuildParams.componentNames,
                                    apiBuildParams.componentMolarWeights,
                                    apiBuildParams.componentCriticalTemperatures,
                                    apiBuildParams.componentCriticalPressures,
                                    apiBuildParams.componentOmegas
                                  ) );
      compositionalSystems.push_back( std::move( pair ) );
      return compositionalSystems.back().second.get();
    }
  }
};

static Holder holder;

void validatePublicApi( const std::string & json_string )
{
  const json j = json::parse( json_string );

  const auto refMsp = j.at( PublicAPIKeys::OUTPUT ).get< pds::PDSMSP >();

  const json & computation = j.at( PublicAPIKeys::INPUT ).at( PublicAPIKeys::COMPUTATION );
  const double pressure = computation.at( PublicAPIKeys::Computation::PRESSURE ).get< double >();
  const double temperature = computation.at( PublicAPIKeys::Computation::TEMPERATURE ).get< double >();
  const std::vector< double > feed = computation.at( PublicAPIKeys::Computation::FEED ).get< std::vector< double > >();

  pds::FLASH_TYPE flashType = j.at( PublicAPIKeys::INPUT ).at( PublicAPIKeys::API ).at( FlashKeys::TYPE ).get< pds::FLASH_TYPE >();


  pvt::MultiphaseSystem * multiphaseSystem = nullptr;
  if( flashType == pds::FLASH_TYPE::NEGATIVE_TWO_PHASE or
      flashType == pds::FLASH_TYPE::FREE_WATER or
      flashType == pds::FLASH_TYPE::TRIVIAL )
  {
    pds::CompositionalApiInputs const apiInputs = j.at( PublicAPIKeys::INPUT ).at( PublicAPIKeys::API ).get< pds::CompositionalApiInputs >();
    multiphaseSystem = holder.getCompositionalSystem( apiInputs );
  }
  if( flashType == pds::FLASH_TYPE::BLACK_OIL or flashType == pds::FLASH_TYPE::DEAD_OIL )
  {
    pds::BlackOilDeadOilApiInputs const apiInputs = j.at( PublicAPIKeys::INPUT ).at( PublicAPIKeys::API ).get< pds::BlackOilDeadOilApiInputs >();
    multiphaseSystem = holder.getDeadOilBlackOilSystem( flashType, apiInputs );
  }

  multiphaseSystem->Update( pressure, temperature, feed );
  pvt::MultiphaseSystemProperties const & actualMsp = multiphaseSystem->getMultiphaseSystemProperties();

  compare( actualMsp, refMsp );

  // Building new json reference, uncomment lines below to display computed values as json.
//  auto newRef = json{
//    { PublicAPIKeys::INPUT,  j.at( PublicAPIKeys::INPUT )},
//    { PublicAPIKeys::OUTPUT, actualMsp }
//  };
//  std::cout << newRef << std::endl;
}

TEST( pvt, publicApi )
{
  // FIXME Is there a simple way to use data providers with gtest?
  const std::vector< std::string > fileNames{
    "pvt_data.txt"
//    "4comp_2ph_1d.log.err",
//    "dead_oil_wells_2d.log.err",
//    "compositional_multiphase_wells_1d.log.err",
//    "compositional_multiphase_wells_2d.log.err",
//    "deadoil_3ph_staircase_3d-10000.log.err", // TODO Waiting for more elements in the table.
//    "blackoil_2ph_1d.log.err"
  };

  // FIXME Fail if file is not found!
  std::string line;
  for( const std::string & fileName: fileNames )
  {
    std::ifstream dataFile( "data/" + fileName );
    while( std::getline( dataFile, line ) )
    {
      const bool isComment = line.rfind( "#", 0 ) == 0;
      if( not line.empty() and not isComment )
      {
        validatePublicApi( line );
      }
    }
  }
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
