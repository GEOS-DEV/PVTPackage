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

#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp"

#include "Utils/math.hpp"

#include <limits>
#include <map>

namespace PVTPackage
{

FreeWaterFlash::FreeWaterFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                                const std::vector< pvt::EOS_TYPE > & eosTypes,
                                ComponentProperties const & componentProperties )
  : CompositionalFlash( phases, eosTypes, componentProperties )
{
  m_WaterIndex = componentProperties.WaterIndex;
}

bool FreeWaterFlash::isThreePhase( const std::vector< double > & kValues,
                                   const std::vector< double > & feed,
                                   const std::list< std::size_t > & nonZeroIndex,
                                   double KWater_GasWater,
                                   double KWater_OilWater,
                                   double waterFeed,
                                   std::size_t waterIndex )
{
  double epsilon = std::numeric_limits< double >::epsilon();

  //Min and Max Kvalues for non-zero composition
  double max_K = 0, min_K = 1 / epsilon;
  for( auto it = nonZeroIndex.begin(); it != nonZeroIndex.end(); ++it )
  {
    if( static_cast< std::size_t >(std::distance( nonZeroIndex.begin(), it )) != waterIndex )
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
  }

  const auto v_bar = -( waterFeed - KWater_OilWater ) / ( KWater_OilWater - KWater_GasWater );
  const auto Klimit = 1.0 - ( KWater_GasWater - KWater_OilWater ) / ( 1.0 - KWater_OilWater );
  if( max_K <= Klimit )
  {
    return KWater_GasWater > KWater_OilWater;
  }

  if( min_K >= Klimit )
  {
    return KWater_GasWater <= KWater_OilWater;
  }

  const auto vmin = ( -1.0 - ( KWater_OilWater - waterFeed ) / ( 1.0 - KWater_OilWater ) )
                    / ( -1.0 + max_K + ( KWater_GasWater - KWater_OilWater ) / ( 1.0 - KWater_OilWater ) );
  const auto vmax = ( -1.0 - ( KWater_OilWater - waterFeed ) / ( 1.0 - KWater_OilWater ) )
                    / ( -1.0 + min_K + ( KWater_GasWater - KWater_OilWater ) / ( 1.0 - KWater_OilWater ) );

  if( v_bar > vmax )
  {
    return KWater_GasWater > KWater_OilWater;
  }

  if( v_bar <= vmin )
  {
    return KWater_GasWater <= KWater_OilWater;
  }

  return modifiedRachfordRiceFunction( kValues, feed, nonZeroIndex, KWater_GasWater, KWater_OilWater, waterFeed, waterIndex, v_bar ) < 0;
}

double FreeWaterFlash::modifiedRachfordRiceFunction( const std::vector< double > & kValues,
                                                     const std::vector< double > & feed,
                                                     const std::list< std::size_t > & nonZeroIndex,
                                                     double kWater_GasWater,
                                                     double kWater_OilWater,
                                                     double waterFeed,
                                                     std::size_t waterIndex,
                                                     double x )
{
  double val = 0;
  double kStarW = ( 1.0 - kWater_GasWater ) / ( 1.0 - kWater_OilWater );
  double K_z_w = ( 1.0 - waterFeed ) / ( 1.0 - kWater_OilWater );

  for( auto ic : nonZeroIndex )
  {
    if( ic != waterIndex )
    {
      const double K = ( kValues[ic] - kStarW );
      val = val + feed[ic] * K / ( K_z_w + x * K );
    }
  }

  return val;
}

double FreeWaterFlash::dModifiedRachfordRiceFunction_dx( const std::vector< double > & kValues,
                                                         const std::vector< double > & feed,
                                                         const std::list< std::size_t > & nonZeroIndex,
                                                         const double kWater_GasWater,
                                                         const double kWater_OilWater,
                                                         double waterFeed,
                                                         std::size_t waterIndex,
                                                         double x )
{
  double val = 0;
  double kStarW = ( 1.0 - kWater_GasWater ) / ( 1.0 - kWater_OilWater );
  double K_z_w = ( 1.0 - waterFeed ) / ( 1.0 - kWater_OilWater );
  for( auto ic : nonZeroIndex )
  {
    if( ic != waterIndex )
    {
      const double K = ( kValues[ic] - kStarW );
      const double R = K / ( K_z_w + x * K );
      val = val - feed[ic] * R * R;
    }
  }
  return val;
}

double FreeWaterFlash::solveModifiedRachfordRiceEquation( const std::vector< double > & kValues,
                                                          const std::vector< double > & feed,
                                                          const std::list< std::size_t > & nonZeroIndex,
                                                          double kWater_gasWater,
                                                          double kWater_oilWater,
                                                          double waterFeed,
                                                          std::size_t waterIndex )
{
  double gas_phase_mole_fraction = 0;

  //Numerical Parameters //TODO: move them outside the function
  double SSI_tolerance = 1e-8;
  int max_SSI_iterations = 200;
  double Newton_tolerance = 1e-12;
  int max_Newton_iterations = 30;
  double epsilon = std::numeric_limits< double >::epsilon();

  //Min and Max Kvalues for non-zero composition
  double max_K = 0, min_K = 1 / epsilon;
  for( auto it = nonZeroIndex.begin(); it != nonZeroIndex.end(); ++it )
  {
    if( static_cast< std::size_t >(std::distance( nonZeroIndex.begin(), it )) != waterIndex )
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
  }

  //Check for trivial solutions. This corresponds to bad Kvalues //TODO:to be fixed
  if( max_K < ( 1.0 - ( kWater_gasWater - kWater_oilWater ) / ( 1.0 - kWater_oilWater ) ) )
  {
    return gas_phase_mole_fraction = 0.0;
  }
  if( min_K > ( 1.0 - ( kWater_gasWater - kWater_oilWater ) / ( 1.0 - kWater_oilWater ) ) )
  {
    return gas_phase_mole_fraction = 1.0;
  }

  //Solver
  //Find solution window
  double x_min = ( -1.0 - ( kWater_oilWater - waterFeed ) / ( 1.0 - kWater_oilWater ) )
                 / ( -1.0 + max_K + ( kWater_gasWater - kWater_oilWater ) / ( 1.0 - kWater_oilWater ) );
  double x_max = ( -1.0 - ( kWater_oilWater - waterFeed ) / ( 1.0 - kWater_oilWater ) )
                 / ( -1.0 + min_K + ( kWater_gasWater - kWater_oilWater ) / ( 1.0 - kWater_oilWater ) );
  double sqrt_epsilon = sqrt( epsilon );
  x_min = x_min + sqrt_epsilon * ( std::fabs( x_min ) + sqrt_epsilon );
  x_max = x_max - sqrt_epsilon * ( std::fabs( x_max ) + sqrt_epsilon );

  double current_error = 1 / epsilon;

  //SSI loop
  double func_x_min = 0, func_x_mid = 0, func_x_max = 0;
  int SSI_iteration = 0;
  while( ( current_error > SSI_tolerance ) && ( SSI_iteration < max_SSI_iterations ) )
  {
    double x_mid = 0.5 * ( x_min + x_max );
    func_x_min = 0;
    func_x_mid = 0;
    func_x_max = 0;

    for( auto it = nonZeroIndex.begin(); it != nonZeroIndex.end(); ++it )
    {
      func_x_min = modifiedRachfordRiceFunction( kValues, feed, nonZeroIndex, kWater_gasWater, kWater_oilWater, waterFeed, waterIndex, x_min );
      func_x_mid = modifiedRachfordRiceFunction( kValues, feed, nonZeroIndex, kWater_gasWater, kWater_oilWater, waterFeed, waterIndex, x_mid );
      func_x_max = modifiedRachfordRiceFunction( kValues, feed, nonZeroIndex, kWater_gasWater, kWater_oilWater, waterFeed, waterIndex, x_max );
    }

    ASSERT( !std::isnan( func_x_min ), "Modified Rachford-Rice solver returns NaN" );
    ASSERT( !std::isnan( func_x_mid ), "Modified Rachford-Rice solver returns NaN" );
    ASSERT( !std::isnan( func_x_max ), "Modified Rachford-Rice solver returns NaN" );

    if( ( func_x_min < 0 ) && ( func_x_max < 0 ) )
    {
      return gas_phase_mole_fraction = 0.0;
    }

    if( ( func_x_min > 1 ) && ( func_x_max > 1 ) )
    {
      return gas_phase_mole_fraction = 1.0;
    }

    if( func_x_min * func_x_mid < 0.0 )
    {
      x_max = x_mid;
    }

    if( func_x_max * func_x_mid < 0.0 )
    {
      x_min = x_mid;
    }

    current_error = std::min( std::fabs( func_x_max - func_x_min ), std::fabs( x_max - x_min ) );

    SSI_iteration++;

    if( SSI_iteration == max_SSI_iterations )
    {
      LOGWARNING( "Rachford-Rice SSI reached max number of iterations" );
    }
  }
  gas_phase_mole_fraction = 0.5 * ( x_max + x_min );

  //Newton loop
  int Newton_iteration = 0;
  double Newton_value = gas_phase_mole_fraction;
  while( ( current_error > Newton_tolerance ) && ( Newton_iteration < max_Newton_iterations ) )
  {
    double delta_Newton =
      -modifiedRachfordRiceFunction( kValues, feed, nonZeroIndex, kWater_gasWater, kWater_oilWater, waterFeed, waterIndex, Newton_value )
      / dModifiedRachfordRiceFunction_dx( kValues, feed, nonZeroIndex, kWater_gasWater, kWater_oilWater, waterFeed, waterIndex, Newton_value );

    current_error = std::fabs( delta_Newton ) / std::fabs( Newton_value );
    Newton_value = Newton_value + delta_Newton;
    Newton_iteration++;

    if( Newton_iteration == max_Newton_iterations )
    {
      LOGWARNING( "Rachford-Rice Newton reached max number of iterations" );
    }
  }
  return gas_phase_mole_fraction = Newton_value;
}

bool FreeWaterFlash::computeEquilibrium( FreeWaterFlashMultiphaseSystemProperties & sysProps ) const
{
  // Equilibrium convergence parameters
  const int max_SSI_iterations = 100;
  const double fugacityEpsilon = 1e-8;

  const auto & pressure = sysProps.getPressure();
  const auto & temperature = sysProps.getTemperature();
  const std::vector< double > & feed = sysProps.getFeed();

  ASSERT( std::fabs( math::sum_array( feed ) - 1.0 ) < 1e-12, "Feed sum must be 1" );

  const std::size_t nComponents = getNComponents();

  // Water
  const std::size_t waterIndex = getWaterIndex();
  const auto & waterFeed = feed[waterIndex];

  const std::vector< double > & oilLnFugacity = sysProps.getOilLnFugacity();
  const std::vector< double > & gasLnFugacity = sysProps.getGasLnFugacity();
  const std::vector< double > & waterLnFugacity = sysProps.getWaterLnFugacity();

  std::vector< double > fugacityRatios( nComponents ), fugacityRatiosW( nComponents );

  // Compute Equilibrium ratios
  std::vector< double > kGasLiquid = computeWilsonGasLiquidKvalue( pressure, temperature );
  kGasLiquid[waterIndex] = std::numeric_limits< double >::max(); //std::numeric_limits<double>::infinity(); //No water in oil
  double kWater_GasWater = computeWaterGasKvalue( pressure, temperature )[waterIndex];
  const double kWater_OilWater = 0.0;

  // Check for machine-zero feed values
  const double epsilon = std::numeric_limits< double >::epsilon();
  std::list< std::size_t > positiveComponents;
  for( std::size_t i = 0; i != nComponents; ++i )
  {
    if( feed[i] > epsilon )
    {
      positiveComponents.push_back( i );
    }
  }

  std::vector< double > oilMoleComposition( nComponents, 0. );
  std::vector< double > gasMoleComposition( nComponents, 0. );
  std::vector< double > waterMoleComposition( nComponents, 0. );
  // This map holds references to the data so we can use them in loops
  std::map< pvt::PHASE_TYPE, const std::vector< double > & > moleComposition{
    { pvt::PHASE_TYPE::OIL,               oilMoleComposition },
    { pvt::PHASE_TYPE::GAS,               gasMoleComposition },
    { pvt::PHASE_TYPE::LIQUID_WATER_RICH, waterMoleComposition }
  };
  double oilPhaseMoleFraction, gasPhaseMoleFraction, waterPhaseMoleFraction;

  gasMoleComposition.assign( nComponents, 0.0 );
  oilMoleComposition.assign( nComponents, 0.0 );
  waterMoleComposition.assign( nComponents, 0.0 );

  bool threePhase = false;
  int totalNbIter = 0;
  for( int iter = 0; iter < max_SSI_iterations; ++iter )
  {

    // Test phase state - 3 or less
    threePhase = isThreePhase( kGasLiquid, feed, positiveComponents, kWater_GasWater, kWater_OilWater, waterFeed, waterIndex );

    double vaporFraction = 0;
    if( threePhase )
    {
      // Solve Modifier Rachford-Rice Equation
      gasPhaseMoleFraction = solveModifiedRachfordRiceEquation( kGasLiquid, feed, positiveComponents, kWater_GasWater, kWater_OilWater, waterFeed, waterIndex );

      // Assign phase compositions

      double kStarW = ( 1.0 - kWater_GasWater ) / ( 1.0 - kWater_OilWater );
      double K_z_w = ( 1.0 - waterFeed ) / ( 1.0 - kWater_OilWater );

      for( auto ic : positiveComponents )
      {
        if( ic != waterIndex )
        {
          const double K = ( kGasLiquid[ic] - kStarW );
          oilMoleComposition[ic] = feed[ic] / ( K_z_w + gasPhaseMoleFraction * K );
          gasMoleComposition[ic] = kGasLiquid[ic] * oilMoleComposition[ic];
        }
      }
      oilMoleComposition[waterIndex] = kWater_OilWater;
      gasMoleComposition[waterIndex] = kWater_GasWater;
    }
    else
    {
      // Solve Rachford-Rice Equation
      vaporFraction = solveRachfordRiceEquation( kGasLiquid, feed, positiveComponents );
      gasPhaseMoleFraction = vaporFraction;
      oilPhaseMoleFraction = 1.0 - gasPhaseMoleFraction;
      waterPhaseMoleFraction = 0.0;

      // Assign phase compositions
      for( auto ic : positiveComponents )
      {
        oilMoleComposition[ic] = feed[ic] / ( 1.0 + vaporFraction * ( kGasLiquid[ic] - 1.0 ) );
        gasMoleComposition[ic] = kGasLiquid[ic] * oilMoleComposition[ic];
      }

      //auto KWater_GasWater_max = water_feed / (gas_fraction*(1.0 - KWater_OilWater) + KWater_OilWater);
    }

    oilMoleComposition = math::Normalize( oilMoleComposition );
    gasMoleComposition = math::Normalize( gasMoleComposition );
    waterMoleComposition[waterIndex] = 1.0;

    // Compute phase fugacity
    for( const pvt::PHASE_TYPE phase: sysProps.getPhases() )
    {
      const CubicEoSPhaseModel & model = getCubicEoSPhaseModel( phase );
      const auto props = model.computeAllProperties( pressure, temperature, moleComposition.at( phase ) );
      sysProps.setModelProperties( phase, props );
    }

    // Compute fugacity ratio and check convergence
    bool converged = true;

    for( auto ic : positiveComponents )
    {
      if( ic != waterIndex )
      {
        fugacityRatios[ic] = std::exp( oilLnFugacity[ic] - gasLnFugacity[ic] ) * oilMoleComposition[ic] / gasMoleComposition[ic];
        if( std::fabs( fugacityRatios[ic] - 1.0 ) > fugacityEpsilon )
        {
          converged = false;
        }
      }
      else
      {
        fugacityRatiosW[ic] = std::exp( waterLnFugacity[ic] - gasLnFugacity[ic] ) * waterMoleComposition[ic] / gasMoleComposition[ic];
      }
    }

    if( converged )
    {
      break;
    }

    // Update K-values
    for( auto ic : positiveComponents )
    {
      if( ic != waterIndex )
      {
        kGasLiquid[ic] *= fugacityRatios[ic];
      }
      else
      {
        kWater_GasWater *= fugacityRatiosW[ic];   //HACK: Only this water k-value is updated
      }
    }

    totalNbIter = iter;
  }

  // Retrieve physical bounds from negative flash values
  if( threePhase )
  {
    const double maxGasFraction = ( 1.0 - waterFeed ) / ( 1.0 - kWater_GasWater );

    if( gasPhaseMoleFraction <= 0.0 or gasPhaseMoleFraction >= maxGasFraction )
    {
      pvt::PHASE_TYPE phase;
      if( gasPhaseMoleFraction >= maxGasFraction )
      {
        gasPhaseMoleFraction = maxGasFraction;
        waterPhaseMoleFraction = ( waterFeed + gasPhaseMoleFraction + ( kWater_OilWater - kWater_GasWater ) - kWater_OilWater )
                                 / ( 1.0 - kWater_OilWater );
        for( auto ic : positiveComponents )
        {
          gasMoleComposition[ic] = feed[ic] / gasPhaseMoleFraction;
        }
        gasMoleComposition[waterIndex] = ( waterFeed - waterPhaseMoleFraction ) / gasPhaseMoleFraction;
        phase = pvt::PHASE_TYPE::GAS;
      }
      else
      {
        gasPhaseMoleFraction = 0.0;
        oilMoleComposition = feed;
        phase = pvt::PHASE_TYPE::OIL;
      }

      // Update phase properties since adjusting composition
      const CubicEoSPhaseModel & model = getCubicEoSPhaseModel( phase );
      const auto props = model.computeAllProperties( pressure, temperature, moleComposition.at( phase ) );
      sysProps.setModelProperties( phase, props );
    }

    waterPhaseMoleFraction = ( waterFeed + gasPhaseMoleFraction + ( kWater_OilWater - kWater_GasWater ) - kWater_OilWater )
                             / ( 1.0 - kWater_OilWater );
  }
  else
  {
    if( gasPhaseMoleFraction <= 0.0 or gasPhaseMoleFraction >= 1.0 )
    {
      pvt::PHASE_TYPE phase;
      if( gasPhaseMoleFraction >= 1.0 )
      {
        gasPhaseMoleFraction = 1.0;
        gasMoleComposition = feed;
        phase = pvt::PHASE_TYPE::GAS;
      }
      else
      {
        gasPhaseMoleFraction = 0.0;
        oilMoleComposition = feed;
        phase = pvt::PHASE_TYPE::OIL;
      }

      // Update phase properties since adjusting composition
      const CubicEoSPhaseModel & model = getCubicEoSPhaseModel( phase );
      const auto props = model.computeAllProperties( pressure, temperature, moleComposition.at( phase ) );
      sysProps.setModelProperties( phase, props );
    }

    waterPhaseMoleFraction = 0.0;
  }

  oilPhaseMoleFraction = 1.0 - gasPhaseMoleFraction - waterPhaseMoleFraction;

#ifndef NDEBUG
  //Compute global error - This should be ran on DEBUG MODE only
  auto total_error = 0.0;
  for( std::size_t i = 0; i != nComponents; ++i )
  {
    total_error = total_error + feed[i]
                  - ( gasMoleComposition[i] * gasPhaseMoleFraction + waterMoleComposition[i] * waterPhaseMoleFraction + oilMoleComposition[i] * oilPhaseMoleFraction );
  }
  ASSERT( std::abs( total_error ) < epsilon, "Mass conservation problem in flash" );
#endif

  sysProps.setOilMoleComposition( oilMoleComposition );
  sysProps.setGasMoleComposition( gasMoleComposition );
  sysProps.setWaterMoleComposition( waterMoleComposition );

  sysProps.setOilFraction( oilPhaseMoleFraction );
  sysProps.setGasFraction( gasPhaseMoleFraction );
  sysProps.setWaterFraction( waterPhaseMoleFraction );

  return totalNbIter != max_SSI_iterations;
}

}
