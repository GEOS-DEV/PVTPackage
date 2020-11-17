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

#include "BlackOilFlash.hpp"

#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"

#include <cmath>

namespace PVTPackage
{

bool BlackOilFlash::computeEquilibrium( BlackOilFlashMultiphaseSystemProperties & sysProps )
{
  // FIXME hard coded indices everywhere.
  const auto & pressure = sysProps.getPressure();

  const std::vector< double > & feed = sysProps.getFeed();
  const double zo = feed[0], zg = feed[1], zw = feed[2];

  const BlackOil_OilModel & oilPhaseModel = sysProps.getOilPhaseModel();
  const BlackOil_GasModel & gasPhaseModel = sysProps.getGasPhaseModel();
  const BlackOil_WaterModel & waterPhaseModel = sysProps.getWaterPhaseModel();

  // OIL
  const double & oilSurfaceMoleDensity = oilPhaseModel.getSurfaceMoleDensity();
  const double & oilSurfaceMassDensity = oilPhaseModel.getSurfaceMassDensity();
  const double rsSat = oilPhaseModel.computeRs( pressure );

  // GAS
  const double & gasSurfaceMoleDensity = gasPhaseModel.getSurfaceMoleDensity();
  const double & gasSurfaceMassDensity = gasPhaseModel.getSurfaceMassDensity();
  const double rvSat = gasPhaseModel.computeRv( pressure );

  // Phase State - Negative flash type
  double const Ko = rvSat * ( oilSurfaceMoleDensity + gasSurfaceMoleDensity * rsSat ) / ( gasSurfaceMoleDensity + oilSurfaceMoleDensity * rvSat );
  double const Kg = ( oilSurfaceMoleDensity + gasSurfaceMoleDensity * rsSat ) / ( rsSat * ( gasSurfaceMoleDensity + oilSurfaceMoleDensity * rvSat ) );
  double const V = zo / ( 1. - Kg ) + zg / ( 1. - Ko );

  if( ( 0 < V ) && ( V < 1 ) )  //Two-phase or both oil and gas saturated
  {
    // Phase Fractions
    sysProps.setOilFraction( 1. - V - zw );
    sysProps.setGasFraction( V );
    sysProps.setWaterFraction( zw );
    // OIL
    const double tmpOil = oilSurfaceMoleDensity / ( oilSurfaceMoleDensity + gasSurfaceMoleDensity * rsSat );
    const std::vector< double > oilMoleComposition{ tmpOil, 1. - tmpOil, 0. }; // FIXME always 0.
    sysProps.setOilMoleComposition( oilMoleComposition );

    auto const oilSaturatedProperties = oilPhaseModel.computeSaturatedProperties( pressure, gasSurfaceMoleDensity, gasSurfaceMassDensity );
    sysProps.setOilModelProperties( oilSaturatedProperties );

    // GAS
    const double tmpGas = gasSurfaceMoleDensity / ( gasSurfaceMoleDensity + oilSurfaceMoleDensity * rvSat );
    const std::vector< double > gasMoleComposition{ 1. - tmpGas, tmpGas, 0. }; // FIXME always 0.
    sysProps.setGasMoleComposition( gasMoleComposition );

    auto const gasSaturatedProperties = gasPhaseModel.computeSaturatedProperties( pressure, oilSurfaceMoleDensity, oilSurfaceMassDensity );
    sysProps.setGasModelProperties( gasSaturatedProperties );

    // Oil ln fugacity
    const std::vector< double > oilLnFugacity{
      std::log( oilMoleComposition[0] * Ko * pressure ),
      std::log( oilMoleComposition[1] * Kg * pressure ),
      std::log( 1. )
    };
    sysProps.setOilLnFugacity( oilLnFugacity );

    // Gas and water ln fugacity
    const std::vector< double > gasAndWaterLnFugacity{
      std::log( gasMoleComposition[0] * pressure ),
      std::log( gasMoleComposition[1] * pressure ),
      std::log( 1. ) // FIXME water fugacities are always std::log( 1. )
    };
    sysProps.setGasLnFugacity( gasAndWaterLnFugacity );
    sysProps.setWaterLnFugacity( gasAndWaterLnFugacity ); // FIXME It's the same values for water and gas?
  }
  else if( V > 1 ) //Only gas or undersaturated gas
  {
    LOGERROR( "Undersaturated gas not supported yet" );
  }
  else // Only oil or undersaturated oil
  {
    // Phase Fractions
    sysProps.setOilFraction( 1. - zw );
    sysProps.setGasFraction( 0. );
    sysProps.setWaterFraction( zw );

    // OIL
    std::vector< double > const oilMoleComposition{ zo, zg, 0. }; // FIXME always 0.
    sysProps.setOilMoleComposition( oilMoleComposition );
    auto const oilUnderSaturatedProperties = oilPhaseModel.computeUnderSaturatedProperties( pressure, oilMoleComposition, gasSurfaceMoleDensity, gasSurfaceMassDensity );
    sysProps.setOilModelProperties( oilUnderSaturatedProperties );

    // LN FUGACITIES
    sysProps.setOilLnFugacity( { std::log( 1. ), std::log( 1. ), std::log( 1. ) } );
    sysProps.setGasLnFugacity( { std::log( 1. ), std::log( 1. ), std::log( 1. ) } );
    sysProps.setWaterLnFugacity( { std::log( 1. ), std::log( 1. ), std::log( 1. ) } );
  }

  // Water
  auto const waterProperties = waterPhaseModel.computeProperties( pressure );
  sysProps.setWaterModelProperties( waterProperties );
  // FIXME be sure that waterComp = {0, 0, 1} is done...

  return true;
}

}
