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

#ifndef PVTPACKAGE_TESTFACTOR_HPP
#define PVTPACKAGE_TESTFACTOR_HPP

#include "refactor/passiveDataStructures/PVTEnums.hpp"
#include "refactor/passiveDataStructures/PropertyAndDerivatives.hpp"
#include "refactor/passiveDataStructures/MultiphaseSystemProperties.hpp"

#include "TestFactor.hpp"

#include "pvt/pvt.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace PVTPackage
{
namespace refactor
{

pvt::COMPOSITIONAL_FLASH_TYPE convert( const pds::FLASH_TYPE & input );

pvt::PHASE_TYPE convert( const pds::PHASE_TYPE & input );

pvt::EOS_TYPE convert( const pds::EOS_TYPE & input );

std::vector< pvt::PHASE_TYPE > convert( const std::vector< pds::PHASE_TYPE > & input );

std::vector< pvt::EOS_TYPE > convert( const std::vector< pds::EOS_TYPE > & input );

template< typename T >
void compare( const pvt::ScalarPropertyAndDerivatives< T > & actual,
              const pds::ScalarPropertyAndDerivatives< T > & expected,
              double eps )
{
  ASSERT_NEAR( actual.value, expected.value, eps );
  ASSERT_NEAR( actual.dT, expected.dT, eps );
  ASSERT_NEAR( actual.dP, expected.dP, eps );

  ASSERT_EQ( actual.dz.size(), expected.dz.size() );
  auto f = [eps]( double a,
                  double b )
  {
    return std::abs( a - b ) < eps;
  };
  const bool test = std::equal( actual.dz.cbegin(), actual.dz.cend(),
                                expected.dz.cbegin(), expected.dz.cend(), f );
  EXPECT_TRUE( test );
}

template< typename T >
void compare( const pvt::VectorPropertyAndDerivatives< T > & actual,
              const pds::VectorPropertyAndDerivatives< T > & expected,
              double eps )
{
  auto f = [eps]( double a,
                  double b )
  {
    return std::abs( a - b ) < eps;
  };

  {
    ASSERT_EQ( actual.value.size(), expected.value.size() );
    const bool test = std::equal( actual.value.cbegin(), actual.value.cend(),
                                  expected.value.cbegin(), expected.value.cend(), f );
    EXPECT_TRUE( test );
  }
  {
    ASSERT_EQ( actual.dP.size(), expected.dP.size() );
    const bool test = std::equal( actual.dP.cbegin(), actual.dP.cend(),
                                  expected.dP.cbegin(), expected.dP.cend(), f );
    EXPECT_TRUE( test );
  }
  {
    ASSERT_EQ( actual.dT.size(), expected.dT.size() );
    const bool test = std::equal( actual.dT.cbegin(), actual.dT.cend(),
                                  expected.dT.cbegin(), expected.dT.cend(), f );
    EXPECT_TRUE( test );
  }
  {
    ASSERT_EQ( actual.dz.size(), expected.dz.size() );
    const std::size_t n = actual.dz.size();
    for( std::size_t i = 0; i < n; ++i )
    {
      const std::vector< double > & ai = actual.dz[i], & ei = expected.dz[i];
      ASSERT_EQ( ai.size(), ei.size() );
      const bool test = std::equal( ai.cbegin(), ai.cend(),
                                    ei.cbegin(), ei.cend(), f );
      EXPECT_TRUE( test );
    }
  }
}

void compare( const pvt::MultiphaseSystemProperties & actual,
              const pds::PDSMSP & expected );

}
}

#endif //PVTPACKAGE_TESTFACTOR_HPP
