/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 TotalEnergies
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include "Assert.hpp"

#include <array>
#include <vector>
#include <type_traits>
#include <numeric>
#include <cmath>

namespace math
{

template< typename Array >
typename Array::value_type sum_array( const Array & args )
{
  return std::accumulate( args.begin(), args.end(), typename Array::value_type( 0 ), std::plus< typename Array::value_type >() );
}

template< typename... Ts, typename std::common_type< int, Ts... >::type * = nullptr >
typename std::common_type< int, Ts... >::type sum( Ts... args )
{
  // With C++17 fold expressions we could just write:
  // return (0 + ... + args);
  using type = typename std::common_type< int, Ts... >::type;
  std::array< type, sizeof...( args ) > temp{ args... };
  return sum_array( temp );
}

template< typename Array >
typename Array::value_type prod_array( const Array & args )
{
  return std::accumulate( args.begin(), args.end(), Array::value_type( 1 ), std::multiplies< typename Array::value_type >() );
}

template< typename... Ts, typename std::common_type< int, Ts... >::type * = nullptr >
typename std::common_type< int, Ts... >::type prod( Ts... args )
{
  // With C++17 fold expressions we could just write:
  // return (0 * ... * args);
  using type = typename std::common_type< int, Ts... >::type;
  std::array< type, sizeof...( args ) > temp{ args... };
  return prod_array( temp );
}

template< typename T >
bool isNonNegative( const T & in )
{
  return in >= 0;
}

template< typename T >
bool isNonNegative( const std::vector< T > & in )
{
  bool res = true;
  for( std::size_t i = 0; i != in.size(); ++i )
  {
    if( in[i] < std::numeric_limits< T >::epsilon() )
    { return false; }
  }
  return res;
}

template< typename T >
bool isLowerThanOne( const std::vector< T > & in )
{
  bool res = true;
  for( std::size_t i = 0; i != in.size(); ++i )
  {
    if( in[i] > 1 )
    { return false; }
  }
  return res;
}

template< typename T >
bool isPositive( const T & in )
{
  return in > std::numeric_limits< T >::epsilon();
}

template< typename T >
bool isPositive( const std::vector< T > & in )
{
  bool res = true;
  for( std::size_t i = 0; i != in.size(); i++ )
  {
    if( in[i] + std::numeric_limits< T >::epsilon() <= 0 )
    { return false; }
  }
  return res;
}

template< typename T >
bool isNearlyEqual( T in,
                    T val,
                    int ulp = 1 )
{
  // Adapt from https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
  return std::abs( in - val ) <= std::numeric_limits< T >::epsilon() * std::abs( in + val ) * ulp || std::abs( in - val ) < std::numeric_limits< T >::min();
}

template< typename T >
bool isNotEqual( const std::vector< T > & in,
                 T val )
{
  bool res = true;
  for( std::size_t i = 0; i != in.size(); i++ )
  {
    if( in[i] == val )
    { return false; }
  }
  return res;
}

template< typename T >
bool isRightSize( unsigned int n,
                  const std::vector< std::vector< T>> & in )
{
  bool res = true;
  for( std::size_t i = 0; i != in.size(); i++ )
  {
    if( in[i].size() != n )
    { return false; }
  }
  return res;
}

template< typename T >
bool isRightSize( unsigned int n,
                  const std::vector< T > & in )
{
  return in.size() == n;
}

template< typename T >
void ExtendtoNDimension( std::vector< T > & in,
                         unsigned int n )
{
  if( in.size() == 1 )
  {
    in.assign( n, in[0] );
  }
}

template< typename T >
std::vector< T > ScalarMultiply( std::vector< T > & in,
                                 T scalar )
{
  std::vector< T > out( in.size() );
  for( auto i = 0; i != in.size(); i++ )
  {
    out[i] = in[i] * scalar;
  }
  return out;
}

template< typename T >
void FindSurrondingIndex( std::vector< T > const & x,
                          T xval,
                          std::size_t & iminus,
                          std::size_t & iplus )
{
  ASSERT( !x.empty(), "Interpolation table is empty" );
  ASSERT( x[0] <= xval, "Input x value iut of range, extrapolation not allowed" );
  //Search for interval
  for( iplus = 1; iplus < x.size() - 1 && x[iplus] < xval; ++iplus )
  { }
  iminus = iplus - 1;
}

template< typename T >
std::vector< T > Normalize( const std::vector< T > & xin )
{
  std::vector< T > xout = xin;
  auto sum = sum_array( xin );
  for( std::size_t n = 0; n != xout.size(); ++n )
  {
    xout[n] = xout[n] / sum;
  }
  return xout;
}


template< typename T >
std::vector< T > Interpolation1( std::vector< T > & xin,
                                 std::vector< T > & yin,
                                 std::vector< T > xout )
{
  //return yout

  ASSERT( xin.size() == yin.size(), "Size mismatch" );

  std::vector< T > yout( xout.size() );

  for( std::size_t n = 0; n != xout.size(); ++n )
  {
    T x = xout[n];
    ASSERT( xin[0] <= x, "Input x out of range, cannot extrapolate below the limits" );
    //LOGWARNING(xin[xin.size() - 1] >= x, "Input x out of range, extrapolation");

    //Search for interval
    std::size_t i_minus, i_plus;
    FindSurrondingIndex( xin, x, i_minus, i_plus );

    if( i_minus == i_plus )
    {
      yout[n] = yin[i_minus];
    }
    else
    {
      //yout[n] = yin[i_minus] * (1 - (x - xin[i_minus]) / (xin[i_plus] - xin[i_minus])) + yin[i_plus] * ((x - xin[i_minus]) / (xin[i_plus] - xin[i_minus]));
      yout[n] = yin[i_minus] * ( xin[i_plus] - x ) / ( xin[i_plus] - xin[i_minus] ) + yin[i_plus] * ( ( x - xin[i_minus] ) / ( xin[i_plus] - xin[i_minus] ) );
      if( yout[n] < 0.0 )
      {
        ASSERT( false, "Negative value" );
      }
    }


  }

  return yout;

}

template< typename T >
T LinearInterpolation( T dminus,
                       T dplus,
                       T xminus,
                       T xplus )
{
  T f = dminus / ( dminus + dplus );
  return f * xplus + ( 1 - f ) * xminus;
}

template< typename T >
T LinearInterpolation( T x1,
                       T y1,
                       T x2,
                       T y2,
                       T x3 )
{
  return LinearInterpolation( x3 - x1, x2 - x3, y1, y2 );
}


template< typename T >
T LogInterpolation( T x1,
                    T y1,
                    T x2,
                    T y2,
                    T x3 )
{
  return LogInterpolation( x3 - x1, x2 - x3, y1, y2 );
}


template< typename T >
T LogInterpolation( T dminus,
                    T dplus,
                    T xminus,
                    T xplus )
{
  T f = log( dminus ) / ( log( dminus ) + log( dplus ) );
  T lnx = f * xplus + ( 1 - f ) * xminus;
  return exp( lnx );
}


template< typename T >
T LinearExtrapolation( T x1,
                       T y1,
                       T x2,
                       T y2,
                       T x3 )
{
  return ( y2 - y1 ) / ( x2 - x1 ) * ( x3 - x2 ) + y2;
}

template< typename T >
T LogExtrapolation( T x1,
                    T y1,
                    T x2,
                    T y2,
                    T x3 )
{
  T lny = ( log( y2 ) - log( y1 ) ) / ( log( x2 ) - log( x1 ) ) * ( log( x3 ) - log( x2 ) ) + log( y2 );
  return exp( lny );
}


template< typename T >
std::vector< T > linspace( T a,
                           T b,
                           std::size_t num )
{
  // create a vector of length num
  std::vector< T > v( num );

  // now assign the values to the vector
  for( std::size_t i = 0; i != num; i++ )
  {
    v[i] = a + i * ( ( b - a ) / ( num - 1 ) );
  }
  return v;
}

}
