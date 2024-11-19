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

#include "DeadOil_PhaseModel.hpp"
#include "BlackOil_Utils.hpp"

#include "Utils/math.hpp"

#include <algorithm>

namespace PVTPackage
{

DeadOil_PhaseModel::DeadOil_PhaseModel( pvt::PHASE_TYPE type,
                                        const std::vector< std::vector< double > > & PVD,
                                        double oilSurfaceMassDensity,
                                        double oilSurfaceMw )
  : m_type( type ),
    m_surfaceMassDensity( 0 ),
    m_surfaceMoleDensity( 0 ),
    m_surfaceMolecularWeight( oilSurfaceMw )
{
  // if the phase is present, PVTD.size() == 3
  if( PVD[0].size() != 3 )
  {
    LOGERROR( "The PVD table must contain 3 columns" );
  }

  // Fill table
  createTable( PVD );

  // Check consistency
  checkTableConsistency();

  // Check consistency
  checkTableConsistency();
  
  // Compute density
  m_surfaceMassDensity = oilSurfaceMassDensity;
  m_surfaceMoleDensity = m_surfaceMassDensity / m_surfaceMolecularWeight;
}

void DeadOil_PhaseModel::checkTableConsistency()
{
  //--Check for the presence of two pressure
  ASSERT( m_PVD.Pressure.size() > 1, "At least two pressure is required" );

  //Check for saturated region
  for( std::size_t i = 0; i < m_PVD.NPoints - 1; ++i )
  {
    if( m_type == pvt::PHASE_TYPE::GAS )
    {
      //Bg must decrease with P
      ASSERT( ( m_PVD.B[i + 1] - m_PVD.B[i] ) < 0, "Bg must increase with P" );
    }
  }
}

void DeadOil_PhaseModel::createTable( const std::vector< std::vector< double>> & PVD )
{
  m_PVD.NPoints = PVD.size() - 1;

  for( std::size_t i = 0; i < PVD.size(); ++i )
  {
    m_PVD.Pressure.resize( PVD.size() );
    m_PVD.B.resize( PVD.size() );
    m_PVD.Viscosity.resize( PVD.size() );

    //Saturated part
    m_PVD.Pressure[i] = PVD[i][0];
    m_PVD.B[i] = PVD[i][1];
    m_PVD.Viscosity[i] = PVD[i][2];
  }

  // find the min and max pressure in the table
  m_maxPressure = *( std::max_element( m_PVD.Pressure.begin(), m_PVD.Pressure.end() ) );
  m_minPressure = *( std::min_element( m_PVD.Pressure.begin(), m_PVD.Pressure.end() ) );
}

void DeadOil_PhaseModel::computeBandVisc( double P,
                                          double & B,
                                          double & visc ) const
{
  ASSERT( ( P < m_maxPressure ) & ( P > m_minPressure ), "Pressure out of table range" );
  std::size_t i_lower_branch, i_upper_branch;
  auto const & P_vec = m_PVD.Pressure;
  auto const & B_vec = m_PVD.B;
  auto const & visc_vec = m_PVD.Viscosity;
  math::FindSurrondingIndex( P_vec, P, i_lower_branch, i_upper_branch );
  B = math::LinearInterpolation( P - P_vec[i_lower_branch], P_vec[i_upper_branch] - P, B_vec[i_lower_branch], B_vec[i_upper_branch] );
  visc = math::LinearInterpolation( P - P_vec[i_lower_branch], P_vec[i_upper_branch] - P, visc_vec[i_lower_branch], visc_vec[i_upper_branch] );
}

double DeadOil_PhaseModel::computeMoleDensity( double B ) const
{
  return 1. / B * ( m_surfaceMoleDensity );
}

double DeadOil_PhaseModel::computeMassDensity( double B ) const
{
  return 1. / B * ( m_surfaceMassDensity );
}

BlackOilDeadOilProperties DeadOil_PhaseModel::computeProperties( double pressure ) const
{
  double B, viscosity;
  computeBandVisc( pressure, B, viscosity );

  return BlackOilDeadOilProperties(
    1. / B * ( m_surfaceMassDensity ),
    1. / B * ( m_surfaceMoleDensity ),
    viscosity
  );
}

}
