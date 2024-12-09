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

#include "BlackOilFlash.hpp"

namespace PVTPackage
{

BlackOilFlash::BlackOilFlash( const std::vector< std::vector< double > > & PVTO,
                              double oilSurfaceMassDensity,
                              double oilSurfaceMolecularWeight,
                              const std::vector< std::vector< double > > & PVTG,
                              double gasSurfaceMassDensity,
                              double gasSurfaceMolecularWeight,
                              const std::vector< double > & PVTW,
                              double waterSurfaceMassDensity,
                              double waterSurfaceMolecularWeight )
  :
  m_oilPhaseModel( PVTO, oilSurfaceMassDensity, oilSurfaceMolecularWeight ),
  m_gasPhaseModel( PVTG, gasSurfaceMassDensity, gasSurfaceMolecularWeight ),
  m_waterPhaseModel( PVTW, waterSurfaceMassDensity, waterSurfaceMolecularWeight )
{

}

bool BlackOilFlash::computeEquilibrium( BlackOilFlashMultiphaseSystemProperties & sysProps ) const
{
  // FIXME hard coded indices everywhere.
  const auto & pressure = sysProps.getPressure();

  const std::vector< double > & feed = sysProps.getFeed();
  const double zo = feed[0], zg = feed[1], zw = feed[2];

  // check feed first, and if only water is present (e.g., pure water injection), do nothing
  if( zw >= 1.0 )
  {
    // phase fractions
    sysProps.setOilFraction( 0.0 );
    sysProps.setGasFraction( 0.0 );
    sysProps.setWaterFraction( zw );

    // compositions
    const std::vector< double > zeroMoleComposition{ 0.0, 0.0, 0.0 };
    sysProps.setOilMoleComposition( zeroMoleComposition );
    sysProps.setGasMoleComposition( zeroMoleComposition );
    
    // density, viscosity
    BlackOilDeadOilProperties tmp( 0.0, 0.0, 0.0 );
    sysProps.setOilModelProperties( tmp );
    sysProps.setGasModelProperties( tmp );

    // water
    auto const waterProperties = m_waterPhaseModel.computeProperties( pressure );
    sysProps.setWaterModelProperties( waterProperties );
    
    return true;
  }
  else
  {    
    // OIL
    const double & oilSurfaceMoleDensity = m_oilPhaseModel.getSurfaceMoleDensity();
    const double & oilSurfaceMassDensity = m_oilPhaseModel.getSurfaceMassDensity();
    const double rsSat = m_oilPhaseModel.computeRs( pressure );
    
    // GAS
    const double & gasSurfaceMoleDensity = m_gasPhaseModel.getSurfaceMoleDensity();
    const double & gasSurfaceMassDensity = m_gasPhaseModel.getSurfaceMassDensity();
    const double rvSat = m_gasPhaseModel.computeRv( pressure );

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

      auto const oilSaturatedProperties = m_oilPhaseModel.computeSaturatedProperties( pressure, gasSurfaceMoleDensity, gasSurfaceMassDensity );
      sysProps.setOilModelProperties( oilSaturatedProperties );
    
      // GAS
      const double tmpGas = gasSurfaceMoleDensity / ( gasSurfaceMoleDensity + oilSurfaceMoleDensity * rvSat );
      const std::vector< double > gasMoleComposition{ 1. - tmpGas, tmpGas, 0. }; // FIXME always 0.
      sysProps.setGasMoleComposition( gasMoleComposition );
      
      auto const gasSaturatedProperties = m_gasPhaseModel.computeSaturatedProperties( pressure, oilSurfaceMoleDensity, oilSurfaceMassDensity );
      sysProps.setGasModelProperties( gasSaturatedProperties );

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
      auto const oilUnderSaturatedProperties = m_oilPhaseModel.computeUnderSaturatedProperties( pressure, oilMoleComposition, gasSurfaceMoleDensity, gasSurfaceMassDensity );
      sysProps.setOilModelProperties( oilUnderSaturatedProperties );
    }

    // Water
    auto const waterProperties = m_waterPhaseModel.computeProperties( pressure );
    sysProps.setWaterModelProperties( waterProperties );
    // FIXME be sure that waterComp = {0, 0, 1} is done...

    return true;
  }
}

}
