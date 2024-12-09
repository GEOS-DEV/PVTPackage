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

#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/NegativeTwoPhaseFlashMultiphaseSystemProperties.hpp"

#include "Utils/math.hpp"

#include <limits>
#include <map>
#include <vector>

namespace PVTPackage
{

NegativeTwoPhaseFlash::NegativeTwoPhaseFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                                              const std::vector< pvt::EOS_TYPE > & eosTypes,
                                              ComponentProperties const & componentProperties )
  : CompositionalFlash( phases, eosTypes, componentProperties )
{ }

bool NegativeTwoPhaseFlash::computeEquilibrium( NegativeTwoPhaseFlashMultiphaseSystemProperties & sysProps ) const
{
  // Equilibrium convergence parameters
  const int max_SSI_iterations = 100;
  const double fugacityEpsilon = 1e-8;

  const double & pressure = sysProps.getPressure();
  const double & temperature = sysProps.getTemperature();
  const std::vector< double > & feed = sysProps.getFeed();

  ASSERT( std::fabs( math::sum_array( feed ) - 1.0 ) < 1e-12, "Feed sum must be 1" );

  const std::size_t nComponents = getNComponents();

  std::vector< double > fugacityRatios( nComponents );
  std::vector< double > kGasOil = computeWilsonGasLiquidKvalue( pressure, temperature );

  //Check for machine-zero feed values
  const double epsilon = std::numeric_limits< double >::epsilon();
  std::list< std::size_t > positiveComponents;
  for( std::size_t i = 0; i < nComponents; ++i )
  {
    if( feed[i] > epsilon )
    {
      positiveComponents.push_back( i );
    }
  }

  std::vector< double > oilMoleComposition( nComponents, 0. ), gasMoleComposition( nComponents, 0. );
  // This map holds references to the data so we can use them in loops
  std::map< pvt::PHASE_TYPE, const std::vector< double > & > moleComposition{
    { pvt::PHASE_TYPE::OIL, oilMoleComposition },
    { pvt::PHASE_TYPE::GAS, gasMoleComposition }
  };
  double oilPhaseMoleFraction, gasPhaseMoleFraction;

  int totalNbIter = 0;
  for( int iter = 0; iter < max_SSI_iterations; ++iter )
  {
    // Solve Rachford-Rice Equation
    const double vaporFraction = solveRachfordRiceEquation( kGasOil, feed, positiveComponents );
    gasPhaseMoleFraction = vaporFraction;
    oilPhaseMoleFraction = 1.0 - gasPhaseMoleFraction;

    // Assign phase compositions
    for( auto ic : positiveComponents )
    {
      oilMoleComposition[ic] = feed[ic] / ( 1.0 + vaporFraction * ( kGasOil[ic] - 1.0 ) );
      gasMoleComposition[ic] = kGasOil[ic] * oilMoleComposition[ic];
    }

    oilMoleComposition = math::Normalize( oilMoleComposition );
    gasMoleComposition = math::Normalize( gasMoleComposition );

    // Compute phase fugacity
    for( const pvt::PHASE_TYPE & phase: sysProps.getPhases() )
    {
      const CubicEoSPhaseModel & model = getCubicEoSPhaseModel( phase );
      const auto props = model.computeAllProperties( pressure, temperature, moleComposition.at( phase ) );
      sysProps.setModelProperties( phase, props );
    }

    // Compute fugacity ratio and check convergence
    bool converged = true;

    const std::vector< double > & oilLnFugacity = sysProps.getOilLnFugacity();
    const std::vector< double > & gasLnFugacity = sysProps.getGasLnFugacity();
    for( auto ic : positiveComponents )
    {
      fugacityRatios[ic] = std::exp( oilLnFugacity[ic] - gasLnFugacity[ic] ) * oilMoleComposition[ic] / gasMoleComposition[ic];
      if( std::fabs( fugacityRatios[ic] - 1.0 ) > fugacityEpsilon )
      {
        converged = false;
      }
    }

    if( converged )
    {
      break;
    }

    // Update K-values
    for( auto ic : positiveComponents )
    {
      kGasOil[ic] *= fugacityRatios[ic];
    }

    totalNbIter = iter;
  }

  // Retrieve physical bounds from negative flash values
  if( gasPhaseMoleFraction <= 0. or gasPhaseMoleFraction >= 1. )
  {
    pvt::PHASE_TYPE phase;
    if( gasPhaseMoleFraction >= 1. )
    {
      gasPhaseMoleFraction = 1.;
      oilPhaseMoleFraction = 0.;
      gasMoleComposition = feed;
      phase = pvt::PHASE_TYPE::GAS;
    }
    else
    {
      gasPhaseMoleFraction = 0.;
      oilPhaseMoleFraction = 1.;
      oilMoleComposition = feed;
      phase = pvt::PHASE_TYPE::OIL;
    }

    // Update phase properties since adjusting composition
    const CubicEoSPhaseModel & model = getCubicEoSPhaseModel( phase );
    const auto props = model.computeAllProperties( pressure, temperature, moleComposition.at( phase ) );
    sysProps.setModelProperties( phase, props );
  }

  // TODO reassign the final values
  sysProps.setOilMoleComposition( oilMoleComposition );
  sysProps.setGasMoleComposition( gasMoleComposition );
  sysProps.setOilFraction( oilPhaseMoleFraction );
  sysProps.setGasFraction( gasPhaseMoleFraction );

  // Compute final phase state
//  setPhaseState( sysProps );

  return totalNbIter < max_SSI_iterations;
}

}
