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

#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "Utils/math.hpp"

#include <vector>

namespace PVTPackage
{

TrivialFlash::TrivialFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                            const std::vector< pvt::EOS_TYPE > & eosTypes,
                            ComponentProperties const & componentProperties )
  : CompositionalFlash( phases, eosTypes, componentProperties )
{

}

bool TrivialFlash::computeEquilibrium( TrivialFlashMultiphaseSystemProperties & sysProps ) const
{
  const double & pressure = sysProps.getPressure();
  const double & temperature = sysProps.getTemperature();
  const std::vector< double > & feed = sysProps.getFeed();

  ASSERT( std::fabs( math::sum_array( feed ) - 1 ) < 1e-12, "Feed sum must be 1" );

  // FIXME These componentsProperties seem the same as EOSPhaseModels'. They should no go to the data part.
  const std::size_t nComponents = getNComponents();

  std::vector< double > gasMoleComposition = std::vector< double >( nComponents, 0. );
  std::vector< double > oilMoleComposition = std::vector< double >( nComponents, 0. );
  std::vector< double > waterMoleComposition = std::vector< double >( nComponents, 0. );

  std::vector< double > kGasOil = computeWilsonGasLiquidKvalue( pressure, temperature );
  std::vector< double > kWaterGas = computeWaterGasKvalue( pressure, temperature );
  std::vector< double > kWaterOil = computeWaterOilKvalue( pressure, temperature );

  //Trivial split
  double vOil = 0.;
  double vGas = 0.;
  double vWater = 0.;

  const std::size_t waterIndex = getWaterIndex();
  for( std::size_t i = 0; i != nComponents; ++i )
  {
    if( i != waterIndex )
    {
      //gas-oil
      if( kGasOil[i] > 1 )
      {
        gasMoleComposition[i] = feed[i];
        vGas = vGas + feed[i];
        sysProps.setGasFraction( vGas );
      }
      else
      {
        oilMoleComposition[i] = feed[i];
        vOil = vOil + feed[i];
        sysProps.setOilFraction( vOil );
      }
    }
    else
    {
      //water-gas
      if( kWaterGas[i] < 1 )
      {
        waterMoleComposition[i] = feed[i];
        vWater = vWater + feed[i];
        sysProps.setWaterFraction( vWater );
      }
      else
      {
        gasMoleComposition[i] = feed[i];
        vGas = vGas + feed[i];
        sysProps.setGasFraction( vGas );
      }
    }
  }

  //gas_comp = gas_comp / Vg;
  for( std::size_t i = 0; i != nComponents; ++i )
  {
    gasMoleComposition[i] = gasMoleComposition[i] / vGas;
    oilMoleComposition[i] = oilMoleComposition[i] / vOil;
    waterMoleComposition[i] = waterMoleComposition[i] / vWater;
  }

  sysProps.setGasMoleComposition( gasMoleComposition );
  sysProps.setOilMoleComposition( oilMoleComposition );
  sysProps.setWaterMoleComposition( waterMoleComposition );

  for( const pvt::PHASE_TYPE & phase: sysProps.getPhases() )
  {
    const CubicEoSPhaseModel & model = getCubicEoSPhaseModel( phase );
    const std::vector< double > & moleComposition = sysProps.getMoleComposition( phase ).value;
    const auto props = model.computeAllProperties( pressure, temperature, moleComposition );

    sysProps.setModelProperties( phase, props );
  }

  return true;
}

}
