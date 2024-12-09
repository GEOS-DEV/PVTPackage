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

#include "BlackOil_Utils.hpp"
#include "BlackOil_GasModel.hpp"

#include "Utils/math.hpp"

#include <algorithm>

namespace PVTPackage
{

BlackOil_GasModel::BlackOil_GasModel( const std::vector< std::vector< double > > & PVTG,
                                      double gasSurfaceMassDensity,
                                      double gasSurfaceMw )
  : m_minPressure( -1 ),
    m_maxPressure( -1 ),
    m_surfaceMassDensity( 0 ),
    m_surfaceMoleDensity( 0 ),
    m_surfaceMolecularWeight( gasSurfaceMw )
{

  //--Fill table
  createTable( PVTG );

  //Check Consistency
  //checkTableConsistency();

  ////Create missing undersaturated branches
  //createUnderSaturatedProperties();

  ////Extend existing branches
  //extendUnderSaturatedProperties();

  ////Refine Table branches
  //refineTable(100);

  //Check Consistency
  //checkTableConsistency();


  ///DEBUG PURPOSE - PLOT table for matlab
//		std::ofstream outputFile("PVTG.txt");
//		for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
//		{
//			for (size_t j = 0; j < m_PVTG.UndersaturatedRv[i].size(); ++j)
//			{
//				outputFile << m_PVTG.DewPressure[i] << " " << m_PVTG.UndersaturatedRv[i][j] + m_PVTG.Rv[i] << " " << m_PVTG.UndersaturatedBg[i][j] << " " << m_PVTG.UndersaturatedViscosity[i][j] << "\n";
//			}
//		}


  //Density
  m_surfaceMassDensity = gasSurfaceMassDensity;
  m_surfaceMoleDensity = m_surfaceMassDensity / m_surfaceMolecularWeight;

}

void BlackOil_GasModel::checkTableConsistency() const
{
  //--Check for the presence of one bubble point
  ASSERT( m_PVTG.UndersaturatedRv[m_PVTG.NSaturatedPoints - 1].size() > 1, "At least one bubble pressure is required" );


  //Check for saturated region
  for( std::size_t i = 0; i < m_PVTG.NSaturatedPoints - 1; ++i )
  {
    //Rv must increase with Pdew
    //ASSERT((m_PVTG.Rv[i + 1] - m_PVTG.Rv[i]) > 0, "Rv must increase with Pdew");
    //Bg must decrease with Pdew
    ASSERT( ( m_PVTG.SaturatedBg[i + 1] - m_PVTG.SaturatedBg[i] ) < 0, "Bg must decrease with Pdew in saturated region" );
    //Visc must decrease with Pb
    //ASSERT((m_PVTG.SaturatedViscosity[i + 1] - m_PVTG.SaturatedViscosity[i]) > 0, "Viscosity must increase with Pdew in saturated region");
  }


  //Check for under-saturated branches

  for( std::size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i )
  {

    for( std::size_t j = 0; j < m_PVTG.UndersaturatedRv[i].size() - 1; ++j )
    {
      //Pressure
      ASSERT( ( m_PVTG.UndersaturatedRv[i][j + 1] - m_PVTG.UndersaturatedRv[i][j] ) < 0, "Rv must decrease in undersaturated region" );
    }
  }
}

void BlackOil_GasModel::createTable( const std::vector< std::vector< double > > & PVT )
{
  //Count number of Saturated points
  std::size_t nSaturatedPoints = 0;
  for( std::size_t i = 0; i < PVT.size(); ++i )
  {
    if( PVT[i].size() == 4 )
    {
      nSaturatedPoints++;
    }
  }
  m_PVTG.NSaturatedPoints = nSaturatedPoints;

  //--Store data in a new structure
  m_PVTG.Rv.resize( nSaturatedPoints );
  m_PVTG.DewPressure.resize( nSaturatedPoints );
  m_PVTG.SaturatedBg.resize( nSaturatedPoints );
  m_PVTG.SaturatedViscosity.resize( nSaturatedPoints );
  m_PVTG.UndersaturatedRv.resize( nSaturatedPoints );
  m_PVTG.UndersaturatedBg.resize( nSaturatedPoints );
  m_PVTG.UndersaturatedViscosity.resize( nSaturatedPoints );

  auto i_sat = 0;
  for( std::size_t i = 0; i < PVT.size(); ++i )
  {
    //Saturated part
    m_PVTG.DewPressure[i_sat] = PVT[i][0];
    m_PVTG.Rv[i_sat] = PVT[i][1];
    m_PVTG.SaturatedBg[i_sat] = PVT[i][2];
    m_PVTG.SaturatedViscosity[i_sat] = PVT[i][3];

    //--Undersaturated
    //Add Saturated properties - Pressure= P - Pbub
    m_PVTG.UndersaturatedRv[i_sat].push_back( 0 );
    m_PVTG.UndersaturatedBg[i_sat].push_back( m_PVTG.SaturatedBg[i_sat] );
    m_PVTG.UndersaturatedViscosity[i_sat].push_back( m_PVTG.SaturatedViscosity[i_sat] );

    auto branchSize = 0;
    auto j = i + 1;
    while( PVT[j].size() == 3 )
    {
      ////Assume dry gas, so ignore it
      /*m_PVTG.UndersaturatedRv[i_sat].push_back(PVT[j][0] - m_PVTG.Rv[i_sat]);
      m_PVTG.UndersaturatedBg[i_sat].push_back(PVT[j][1]);
      m_PVTG.UndersaturatedViscosity[i_sat].push_back(PVT[j][2]);*/
      branchSize++;
      j++;
      if( j == PVT.size() )
      {
        break;
      }
    }

    i = i + branchSize;
    i_sat++;
  }

  //Add 1atm value if does not exist yet
  if( !math::isNearlyEqual( m_PVTG.Rv[0], 0. ) )
  {
    auto refPressure = 101325.0;
    const double visc = math::LogExtrapolation( m_PVTG.DewPressure[1], m_PVTG.SaturatedViscosity[1], m_PVTG.DewPressure[0], m_PVTG.SaturatedViscosity[0], refPressure );

    m_PVTG.Rv.insert( m_PVTG.Rv.begin(), 0. );
    m_PVTG.DewPressure.insert( m_PVTG.DewPressure.begin(), refPressure );
    m_PVTG.SaturatedBg.insert( m_PVTG.SaturatedBg.begin(), 1.0 );
    m_PVTG.SaturatedViscosity.insert( m_PVTG.SaturatedViscosity.begin(), visc );
    m_PVTG.NSaturatedPoints++;

    m_PVTG.UndersaturatedRv.insert( m_PVTG.UndersaturatedRv.begin(), { 0 } );
    m_PVTG.UndersaturatedBg.insert( m_PVTG.UndersaturatedBg.begin(), { 1.0 } );
    m_PVTG.UndersaturatedViscosity.insert( m_PVTG.UndersaturatedViscosity.begin(), { visc } );
  }

  m_PVTG.MaxRelativeRv = 0;
  m_PVTG.MinRelativeRv = 1e8;

  for( std::size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i )
  {
    double maxRv = *( std::max_element( m_PVTG.UndersaturatedRv[i].begin(), m_PVTG.UndersaturatedRv[i].end() ) );
    double minRv = *( std::min_element( m_PVTG.UndersaturatedRv[i].begin(), m_PVTG.UndersaturatedRv[i].end() ) );
    m_PVTG.MaxRelativeRv = std::max( maxRv, m_PVTG.MaxRelativeRv );
    m_PVTG.MinRelativeRv = std::min( minRv, m_PVTG.MinRelativeRv );
  }

  //
  m_maxPressure = *( std::max_element( m_PVTG.DewPressure.begin(), m_PVTG.DewPressure.end() ) );
  m_minPressure = *( std::min_element( m_PVTG.DewPressure.begin(), m_PVTG.DewPressure.end() ) );
}

void BlackOil_GasModel::extendUnderSaturatedProperties()
{
  ////Extrapolate Undersaturated properties up to max pressure
  //for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
  //{
  //	auto dPext = m_PVTG.MaxRelativeRv - m_PVTG.UndersaturatedRv[i].back();

  //	const auto& Pusat = m_PVTG.UndersaturatedRv[i];
  //	const auto& Bousat = m_PVTG.UndersaturatedBg[i];
  //	const auto& Viscusat = m_PVTG.UndersaturatedViscosity[i];

  //	if (std::fabs(dPext) > 0)
  //	{
  //		auto branch_size = m_PVTG.UndersaturatedRv[i].size();
  //		auto Bo = math::LinearExtrapolation(Pusat[branch_size - 2], Bousat[branch_size - 2], Pusat[branch_size - 1], Bousat[branch_size - 1], m_PVTG.MaxRelativeRv);
  //		auto Visc = math::LinearExtrapolation(Pusat[branch_size - 2], Viscusat[branch_size - 2], Pusat[branch_size - 1], Viscusat[branch_size - 1], m_PVTG.MaxRelativeRv);
  //		m_PVTG.UndersaturatedBg[i].push_back(Bo);
  //		m_PVTG.UndersaturatedViscosity[i].push_back(Visc);
  //		m_PVTG.UndersaturatedRv[i].push_back(m_PVTG.MaxRelativeRv);
  //	}

  //}
}

void BlackOil_GasModel::refineTable( std::size_t nLevel )
{
  (void) nLevel;
  //std::vector<double> refine_p;
  //for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
  //{
  //	for (size_t j = 0; j < m_PVTG.UndersaturatedRv[i].size(); ++j)
  //	{
  //		refine_p.push_back(m_PVTG.UndersaturatedRv[i][j]);
  //	}
  //}
  //std::sort(refine_p.begin(), refine_p.end());
  //refine_p.erase(std::unique(refine_p.begin(), refine_p.end()), refine_p.end());

  ////Linspace
  //double maxval = *std::max_element(refine_p.begin(), refine_p.end());
  //refine_p = math::linspace(0.0, maxval, nlevel);

  //for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
  //{

  //	//Bo
  //	auto prop_refined = math::Interpolation1(m_PVTG.UndersaturatedRv[i], m_PVTG.UndersaturatedBg[i], refine_p);
  //	m_PVTG.UndersaturatedBg[i].resize(refine_p.size());
  //	m_PVTG.UndersaturatedBg[i] = prop_refined;

  //	//Visc
  //	prop_refined = math::Interpolation1(m_PVTG.UndersaturatedRv[i], m_PVTG.UndersaturatedViscosity[i], refine_p);
  //	m_PVTG.UndersaturatedViscosity[i].resize(refine_p.size());
  //	m_PVTG.UndersaturatedViscosity[i] = prop_refined;


  //	for (size_t j = 0; j < refine_p.size(); ++j)
  //	{
  //		m_PVTG.UndersaturatedRv[i].resize(refine_p.size());
  //		m_PVTG.UndersaturatedRv[i][j] = refine_p[j];
  //	}

  //}
}

double BlackOil_GasModel::computePdew( double Rv ) const
{
  std::size_t i_lower_branch, i_upper_branch;
  math::FindSurrondingIndex( m_PVTG.Rv, Rv, i_lower_branch, i_upper_branch );
  return math::LinearInterpolation( m_PVTG.Rv[i_lower_branch], m_PVTG.DewPressure[i_lower_branch], m_PVTG.Rv[i_upper_branch], m_PVTG.DewPressure[i_upper_branch], Rv );
}

double BlackOil_GasModel::computeRv( double Pdew ) const
{
  ASSERT( ( Pdew < m_maxPressure ) & ( Pdew > m_minPressure ), "Pressure out of table range" );
  std::size_t i_lower_branch, i_upper_branch;
  math::FindSurrondingIndex( m_PVTG.DewPressure, Pdew, i_lower_branch, i_upper_branch );
  return math::LinearInterpolation( m_PVTG.DewPressure[i_lower_branch], m_PVTG.Rv[i_lower_branch], m_PVTG.DewPressure[i_upper_branch], m_PVTG.Rv[i_upper_branch], Pdew );
}

BlackOilDeadOilProperties BlackOil_GasModel::computeSaturatedProperties( double Pdew,
                                                                         double oilMoleSurfaceDensity,
                                                                         double oilMassSurfaceDensity ) const
{
  ASSERT( ( Pdew < m_maxPressure ) & ( Pdew > m_minPressure ), "Pressure out of table range" );
  auto Rv = computeRv( Pdew );
  double Bg, viscosity;
  computeBgVisc( Pdew, Bg, viscosity );

  return BlackOilDeadOilProperties(
    computeMassDensity( Rv, Bg, oilMassSurfaceDensity ),
    computeMoleDensity( Rv, Bg, oilMoleSurfaceDensity ),
    viscosity
  );
}

void BlackOil_GasModel::computeBgVisc( const double & pres,
                                       double & Bg,
                                       double & viscosity ) const
{
  std::size_t i_lower, i_upper;
  auto const & Bg_vec = m_PVTG.SaturatedBg;
  auto const & visc_vec = m_PVTG.SaturatedViscosity;
  math::FindSurrondingIndex( m_PVTG.DewPressure, pres, i_lower, i_upper );
  Bg = math::LinearInterpolation( m_PVTG.DewPressure[i_lower], Bg_vec[i_lower], m_PVTG.DewPressure[i_upper], Bg_vec[i_upper] );
  viscosity = math::LinearInterpolation( m_PVTG.DewPressure[i_lower], visc_vec[i_lower], m_PVTG.DewPressure[i_upper], visc_vec[i_upper] );
}
  
double BlackOil_GasModel::computeMoleDensity( double Rv,
                                              double Bg,
                                              double surfaceOilMoleDensity ) const
{
  return 1. / Bg * ( m_surfaceMoleDensity + surfaceOilMoleDensity * Rv );
}

double BlackOil_GasModel::computeMassDensity( double Rv,
                                              double Bg,
                                              double surfaceOilMassDensity ) const
{
  return 1. / Bg * ( m_surfaceMassDensity + surfaceOilMassDensity * Rv );
}

}
