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

#include "CubicEoSPhaseModel.hpp"

#include "Utils/Logger.hpp"

#include <algorithm>

namespace PVTPackage
{

CubicEoSPhaseModel::Properties CubicEoSPhaseModel::computeAllProperties( double pressure,
                                                                         double temperature,
                                                                         std::vector< double > const & composition ) const
{
  const CubicEosMixtureCoefficients mixtureCoeffs = computeMixtureCoefficients( pressure, temperature, composition );
  const double compressibilityFactor = computeCompressibilityFactor( pressure, temperature, composition, mixtureCoeffs );
  const std::vector< double > lnFugacitiesCoeffs = computeLnFugacitiesCoefficients( composition, compressibilityFactor, mixtureCoeffs );
  const double moleDensity = computeMoleDensity( m_componentProperties, pressure, temperature, composition, compressibilityFactor );
  const double molecularWeight = computeMolecularWeight( m_componentProperties, composition );
  const double massDensity = computeMassDensity( moleDensity, molecularWeight );
  const double viscosity = computeViscosity();
  
  return Properties{
    compressibilityFactor, massDensity, moleDensity, viscosity, molecularWeight, lnFugacitiesCoeffs
  };
}

CubicEoSPhaseModel::CubicEosMixtureCoefficients CubicEoSPhaseModel::computeMixtureCoefficients( double pressure,
                                                                                                double temperature,
                                                                                                std::vector< double > const & composition ) const
{
  auto const & nComponents = m_componentProperties.NComponents;
  std::vector< double > const & Tc = m_componentProperties.Tc;
  std::vector< double > const & Pc = m_componentProperties.Pc;

  CubicEosMixtureCoefficients mixCoeffs( nComponents );

  //Mixture coefficients
  for( std::size_t i = 0; i < nComponents; ++i )
  {
    mixCoeffs.APure[i] = m_omegaA * Tc[i] * Tc[i] * pressure / ( Pc[i] * temperature * temperature ) * pow( 1.0 + m_m[i] * ( 1.0 - sqrt( temperature / Tc[i] ) ), 2.0 );
    mixCoeffs.BPure[i] = m_omegaB * Tc[i] * pressure / ( Pc[i] * temperature );
  }

  mixCoeffs.AMixture = 0;
  mixCoeffs.BMixture = 0;
  for( std::size_t i = 0; i < nComponents; ++i )
  {
    for( std::size_t j = 0; j < nComponents; ++j )
    {
      mixCoeffs.AMixture = mixCoeffs.AMixture + ( composition[i] * composition[j] * ( 1.0 - m_BICs ) * sqrt( mixCoeffs.APure[i] * mixCoeffs.APure[j] ) );
    }
    mixCoeffs.BMixture = mixCoeffs.BMixture + composition[i] * mixCoeffs.BPure[i];
  }

  return mixCoeffs;
}

double CubicEoSPhaseModel::computeCompressibilityFactor( double pressure,
                                                         double temperature,
                                                         std::vector< double > const & composition,
                                                         CubicEosMixtureCoefficients const & mixCoeffs ) const
{
  (void) pressure, (void) temperature;
  //ASSERT(m_MixtureCoefficientsUpToDate, "Z factor requires mixture properties up-to-date.");
  std::vector< double > coeff( 3 );
  //aZ3+bZ2+cZ+d=0
  double a = 1.0;
  double b = ( m_delta1 + m_delta2 - 1.0 ) * mixCoeffs.BMixture - 1.0;
  double c = mixCoeffs.AMixture + m_delta1 * m_delta2 * mixCoeffs.BMixture * mixCoeffs.BMixture - ( m_delta1 + m_delta2 ) * mixCoeffs.BMixture * ( mixCoeffs.BMixture + 1.0 );
  double d = -( mixCoeffs.AMixture * mixCoeffs.BMixture + m_delta1 * m_delta2 * mixCoeffs.BMixture * mixCoeffs.BMixture * ( mixCoeffs.BMixture + 1.0 ) );

  auto sols = solveCubicPolynomial( a, b, c, d );
  double compressibility;


//		if ((m_PhaseType == pvt::PHASE_TYPE::LIQUID_WATER_RICH) || (m_PhaseType == pvt::PHASE_TYPE::OIL))
//		{
//			compressibility = *(std::min_element(sols.begin(), sols.end()));
//		}
//		else if (m_PhaseType == pvt::PHASE_TYPE::GAS)
//		{
//			compressibility = *(std::max_element(sols.begin(), sols.end()));
//		}
//		else
//		{
//			LOGERROR("Undetermined phase type for compressibility calculation");
//			compressibility = -1;
//		}

  if( sols.size() == 1 )
  {
    compressibility = sols[0];
  }
  else
  {

    // Check for unphysical roots and remove them 
    auto const unphysical = [&]( double v ) { return v <= mixCoeffs.BMixture; };
    sols.erase( std::remove_if( sols.begin(), sols.end(), unphysical ), sols.end() );

    // Choose the root according to Gibbs' free energy minimization
    const double Zmin = *std::min_element( sols.begin(), sols.end() );
    const double Zmax = *std::max_element( sols.begin(), sols.end() );

    auto ln_fug_min = computeLnFugacitiesCoefficients( composition, Zmin, mixCoeffs );
    auto ln_fug_max = computeLnFugacitiesCoefficients( composition, Zmax, mixCoeffs );

    double dG = 0.0;
    for( std::size_t ic = 0; ic < m_componentProperties.NComponents; ++ic )
    {
      dG += composition[ic] * ( ln_fug_min[ic] - ln_fug_max[ic] );
    }

    compressibility = ( dG < 0 ) ? Zmin : Zmax;
  }

  return compressibility;
}

std::vector< double > CubicEoSPhaseModel::computeLnFugacitiesCoefficients( std::vector< double > const & composition,
                                                                           double Z,
                                                                           CubicEosMixtureCoefficients const & mixtureCoefficients ) const
{
  auto const nComponents = m_componentProperties.NComponents;

  auto ki = std::vector< double >( nComponents, 0 );

  std::vector< double > lnFugacityCoeffs( nComponents );

  //Ki
  for( std::size_t i = 0; i < nComponents; ++i )
  {
    for( std::size_t j = 0; j < nComponents; ++j )
    {
      ki[i] = ki[i] + composition[j] * ( 1.0 - m_BICs ) * sqrt( mixtureCoefficients.APure[i] * mixtureCoefficients.APure[j] );
    }
  }

  //E
  const double E = log( ( Z + m_delta1 * mixtureCoefficients.BMixture ) / ( Z + m_delta2 * mixtureCoefficients.BMixture ) );
  const double F = log( Z - mixtureCoefficients.BMixture );
  const double G = 1.0 / ( ( m_delta1 - m_delta2 ) * mixtureCoefficients.BMixture );
  const double A = mixtureCoefficients.AMixture;

  //Ln phi
  for( std::size_t i = 0; i < nComponents; ++i )
  {
    const double B = mixtureCoefficients.BPure[i] / mixtureCoefficients.BMixture;
    lnFugacityCoeffs[i] = ( Z - 1 ) * B - F - G * ( 2 * ki[i] - A * B ) * E;
  }

  return lnFugacityCoeffs;
}

double CubicEoSPhaseModel::computeMoleDensity( const ComponentProperties & componentProperties,
                                               double pressure,
                                               double temperature,
                                               std::vector< double > const & composition,
                                               double Z )
{
  auto const & nComponents = componentProperties.NComponents;
  auto const & Vs = componentProperties.VolumeShift;
  double vEos = R * temperature * Z / pressure;
  double vCorrected = vEos;

  double moleDensity;

  for( std::size_t i = 0; i < nComponents; i++ )
  {
    vCorrected = vCorrected + composition[i] * ( ( Vs[i][1] ) * temperature + Vs[i][1] );
  }

  if( std::fabs( vCorrected ) > 0.0 )
  {
    moleDensity = 1.0 / vCorrected;
  }
  else
  {
    moleDensity = 0.0;
  }

  return moleDensity;
}

double CubicEoSPhaseModel::computeMolecularWeight( const ComponentProperties & componentProperties,
                                                   std::vector< double > const & composition )
{
  auto nComponents = componentProperties.NComponents;
  double Mw = 0;
  for( std::size_t i = 0; i < nComponents; i++ )
  {
    Mw = Mw + componentProperties.Mw[i] * composition[i];
  }
  return Mw;
}

double CubicEoSPhaseModel::computeMassDensity( double moleDensity,
                                               double mw )
{
  return moleDensity * mw;
}

double CubicEoSPhaseModel::computeViscosity()
{
  // TODO: implement correlations here
  // needs componentProperties, pressure, temperature, composition
  return 0.001; 
}

void CubicEoSPhaseModel::init()
{
  auto const & nComponents = m_componentProperties.NComponents;
  auto const & omega = m_componentProperties.Omega;

  // Set EOS constants
  switch( m_EOSType )
  {
    case pvt::EOS_TYPE::PENG_ROBINSON:
      m_omegaA = 0.457235529;
      m_omegaB = 0.077796074;
      m_delta1 = 1 + sqrt( 2 );
      m_delta2 = 1 - sqrt( 2 );
      EOS_m_function = &CubicEoSPhaseModel::m_function_PR;
      break;
    case pvt::EOS_TYPE::REDLICH_KWONG_SOAVE:
      m_omegaA = 0.42748;
      m_omegaB = 0.08664;
      m_delta1 = 0.0;
      m_delta2 = 1;
      EOS_m_function = &CubicEoSPhaseModel::m_function_SRK;
      break;
    default:
      LOGERROR( "non supported equation of state" );
      break;
  }

  //Set Constant properties
  m_m.resize( nComponents, 0 );

  for( std::size_t i = 0; i < nComponents; i++ )
  {
    m_m[i] = ( this->*EOS_m_function )( omega[i] );
  }
}

std::vector< double > CubicEoSPhaseModel::solveCubicPolynomial( double m3,
                                                                double m2,
                                                                double m1,
                                                                double m0 )
{
  ////CUBIC EQUATION :  m3 * x^3 +  m3 * x^2 + m1 *x + m0  = 0
  double x1, x2, x3;

  double a1 = m2 / m3;
  double a2 = m1 / m3;
  double a3 = m0 / m3;

  double Q = ( a1 * a1 - 3 * a2 ) / 9;
  double r = ( 2 * a1 * a1 * a1 - 9 * a1 * a2 + 27 * a3 ) / 54;
  double Qcubed = Q * Q * Q;
  double d = Qcubed - r * r;

  /* Three real roots */
  if( d >= 0 )
  {
    double theta = acos( r / sqrt( Qcubed ) );
    double sqrtQ = sqrt( Q );
    x1 = -2 * sqrtQ * cos( theta / 3 ) - a1 / 3;
    x2 = -2 * sqrtQ * cos( ( theta + 2 * PI ) / 3 ) - a1 / 3;
    x3 = -2 * sqrtQ * cos( ( theta + 4 * PI ) / 3 ) - a1 / 3;
    return { x1, x2, x3 };
  }
    /* One real root */
  else
  {
    double e = pow( sqrt( -d ) + fabs( r ), 1. / 3. );
    if( r > 0 )
    {
      e = -e;
    }
    x1 = ( e + Q / e ) - a1 / 3.;
    return { x1 };
  }
}

double CubicEoSPhaseModel::m_function_PR( double omega )
{
  if( omega < 0.49 )
  {
    return 0.37464 + 1.54226 * omega - 0.26992 * omega * omega;
  }
  else
  {
    return 0.3796 + 1.485 * omega - 0.164423 * omega * omega + 0.016666 * omega * omega * omega;
  }
}

double CubicEoSPhaseModel::m_function_SRK( double omega )
{
  return 0.480 + 1.574 * omega - 0.176 * omega * omega;
}

}
