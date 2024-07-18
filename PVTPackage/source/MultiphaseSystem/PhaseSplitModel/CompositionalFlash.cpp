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

#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

#include "Utils/math.hpp"

#include <algorithm>

namespace PVTPackage
{

CompositionalFlash::CompositionalFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                                        const std::vector< pvt::EOS_TYPE > & eosTypes,
                                        ComponentProperties const & componentProperties )
  : m_componentProperties( componentProperties ) // FIXME still usefull?
{
  for( std::size_t i = 0; i != phases.size(); ++i )
  {
    m_phaseModels.insert(
      { phases[i], CubicEoSPhaseModel( m_componentProperties, eosTypes[i], phases[i] ) }
    );
  }
}

double CompositionalFlash::solveRachfordRiceEquation( const std::vector< double > & kValues,
                                                      const std::vector< double > & feed,
                                                      const std::list< std::size_t > & nonZeroIndex )
{
  double gas_phase_mole_fraction = 0;

  //Numerical Parameters //TODO: move them outside the function
  double SSI_tolerance = 1e-3;
  int max_SSI_iterations = 200;
  double Newton_tolerance = 1e-12;
  int max_Newton_iterations = 30;
  double epsilon = std::numeric_limits< double >::epsilon();

  //Min and Max Kvalues for non-zero composition
  double max_K = 0, min_K = 1 / epsilon;
  for( auto it = nonZeroIndex.begin(); it != nonZeroIndex.end(); ++it )
  {
    if( kValues[*it] > max_K )
    {
      max_K = kValues[*it];
    }
    if( kValues[*it] < min_K )
    {
      min_K = kValues[*it];
    }
  }

  //Check for trivial solutions. This corresponds to bad Kvalues //TODO:to be fixed
  if( max_K < 1.0 )
  {
    return gas_phase_mole_fraction = 0.0;
  }
  if( min_K > 1.0 )
  {
    return gas_phase_mole_fraction = 1.0;
  }

  //Solver
  //Find solution window
  double x_min = 1.0 / ( 1 - max_K );
  double x_max = 1.0 / ( 1 - min_K );
  double sqrt_epsilon = sqrt( epsilon );
  x_min = x_min + sqrt_epsilon * ( std::fabs( x_min ) + sqrt_epsilon );
  x_max = x_max - sqrt_epsilon * ( std::fabs( x_max ) + sqrt_epsilon );

  double current_error = 1 / epsilon;

  //SSI loop
  double func_x_min = 0, func_x_mid = 0, func_x_max = 0;
  bool recompute_min = true, recompute_max = true;
  int SSI_iteration = 0;
  while( ( current_error > SSI_tolerance ) && ( SSI_iteration < max_SSI_iterations ) )
  {
    double x_mid = 0.5 * ( x_min + x_max );
    if( recompute_min )
    { func_x_min = RachfordRiceFunction( kValues, feed, nonZeroIndex, x_min ); }
    if( recompute_max )
    { func_x_max = RachfordRiceFunction( kValues, feed, nonZeroIndex, x_max ); }
    func_x_mid = RachfordRiceFunction( kValues, feed, nonZeroIndex, x_mid );

    if( ( func_x_min < 0 ) && ( func_x_max < 0 ) )
    {
      return gas_phase_mole_fraction = 0.0;
    }
    else if( ( func_x_min > 1 ) && ( func_x_max > 1 ) )
    {
      return gas_phase_mole_fraction = 1.0;
    }
    else if( func_x_min * func_x_mid < 0.0 )
    {
      x_max = x_mid;
      recompute_max = true;
      recompute_min = false;
    }
    else if( func_x_max * func_x_mid < 0.0 )
    {
      x_min = x_mid;
      recompute_max = false;
      recompute_min = true;
    }

    current_error = std::min( std::fabs( func_x_max - func_x_min ), std::fabs( x_max - x_min ) );
    SSI_iteration++;

    if( SSI_iteration == max_SSI_iterations )
      LOGWARNING( "Rachford-Rice SSI reached max number of iterations" );
  }

  gas_phase_mole_fraction = 0.5 * ( x_max + x_min );

  //Newton loop
  int Newton_iteration = 0;
  double Newton_value = gas_phase_mole_fraction;
  while( ( current_error > Newton_tolerance ) && ( Newton_iteration < max_Newton_iterations ) )
  {
    double delta_Newton = -RachfordRiceFunction( kValues, feed, nonZeroIndex, Newton_value ) / dRachfordRiceFunction_dx( kValues, feed, nonZeroIndex, Newton_value );
    current_error = std::fabs( delta_Newton ) / std::fabs( Newton_value );

    // Test if we are stepping out of the [x_min;x_max] interval
    if( Newton_value + delta_Newton < x_min )
    {
      Newton_value = .5 * ( Newton_value + x_min );
    }
    else if( Newton_value + delta_Newton > x_max )
    {
      Newton_value = .5 * ( Newton_value + x_max );
    }
    else
    {
      Newton_value = Newton_value + delta_Newton;
    }

    Newton_iteration++;

    if( Newton_iteration == max_Newton_iterations )
      LOGWARNING( "Rachford-Rice Newton reached max number of iterations" );
  }

  return gas_phase_mole_fraction = Newton_value;
}

double CompositionalFlash::RachfordRiceFunction( const std::vector< double > & kValues,
                                                 const std::vector< double > & feed,
                                                 const std::list< std::size_t > & nonZeroIndex,
                                                 double x )
{
  double val = 0;
  for( auto ic : nonZeroIndex )
  {
    const double K = ( kValues[ic] - 1.0 );
    val = val + feed[ic] * K / ( 1.0 + x * K );
  }
  return val;
}

double CompositionalFlash::dRachfordRiceFunction_dx( const std::vector< double > & kValues,
                                                     const std::vector< double > & feed,
                                                     const std::list< std::size_t > & nonZeroIndex,
                                                     double x )
{
  double val = 0;
  for( auto ic : nonZeroIndex )
  {
    const double K = ( kValues[ic] - 1.0 );
    const double R = K / ( 1.0 + x * K );
    val = val - feed[ic] * R * R;
  }
  return val;
}

std::vector< double > CompositionalFlash::computeWilsonGasLiquidKvalue( double pressure,
                                                                        double temperature ) const
{
  const auto nbc = m_componentProperties.NComponents;
  const auto & Tc = m_componentProperties.Tc;
  const auto & Pc = m_componentProperties.Pc;
  const auto & Omega = m_componentProperties.Omega;

  std::vector< double > Kval( nbc );

  //Gas-Oil
  for( std::size_t i = 0; i != nbc; i++ )
  {
    Kval[i] = Pc[i] / pressure * exp( 5.37 * ( 1 + Omega[i] ) * ( 1 - Tc[i] / temperature ) );
  }

  return Kval;
}

std::vector< double > CompositionalFlash::computeWaterGasKvalue( double pressure,
                                                                 double temperature ) const
{
  const auto nbc = m_componentProperties.NComponents;
  const auto water_index = m_componentProperties.WaterIndex;
  std::vector< double > Kval( nbc, 0 );
  Kval[water_index] = exp( -4844.168051 / temperature + 12.93022442 ) * 1e5 / pressure;
  return Kval;
}

std::vector< double > CompositionalFlash::computeWaterOilKvalue( double pressure,
                                                                 double temperature ) const
{
  (void) pressure, (void) temperature;
  const auto nbc = m_componentProperties.NComponents;
  return std::vector< double >( nbc, 0 );
}

const CubicEoSPhaseModel & CompositionalFlash::getCubicEoSPhaseModel( pvt::PHASE_TYPE const & phase ) const
{
  return m_phaseModels.at( phase );
}

std::size_t CompositionalFlash::getNComponents() const
{
  return m_componentProperties.NComponents;
}

std::size_t CompositionalFlash::getWaterIndex() const
{
  return m_componentProperties.WaterIndex;
}

}
