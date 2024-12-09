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

#include "MultiphaseSystem/ComponentProperties.hpp"

#include "Utils/Assert.hpp"

#include "pvt/pvt.hpp"

#include <vector>
#include <cmath>

namespace PVTPackage
{

class CubicEoSPhaseModel final
{
public:

  CubicEoSPhaseModel( ComponentProperties componentProperties,
                      pvt::EOS_TYPE eos,
                      pvt::PHASE_TYPE phase )
    : m_componentProperties( componentProperties ),
      m_phase( phase ),
      m_EOSType( eos ),
      m_omegaA( 0 ),
      m_omegaB( 0 ),
      m_delta1( 0 ),
      m_delta2( 0 ),
      EOS_m_function( nullptr ),
      m_BICs( 0 )
  {
    init();
  }

  const ComponentProperties & getComponentsProperties() const
  {
    return m_componentProperties;
  }

  /**
   * @brief Small utility class for better output.
   *
   * @note Modify with care the order since it's using aggregate automatic constructors.
   */
  struct Properties
  {
    double compressibilityFactor;
    double massDensity;
    double moleDensity;
    double viscosity;
    double molecularWeight;
    std::vector< double > lnFugacityCoefficients;
  };

  Properties computeAllProperties( double pressure,
                                   double temperature,
                                   std::vector< double > const & composition ) const;

private:

  const ComponentProperties m_componentProperties;

  // Constants
  static constexpr double R = 8.3144621;
  static constexpr double PI = 3.141592653589793238462643383279502884;

  // Phase Type
  const pvt::PHASE_TYPE m_phase;

  // EOS parameters
  const pvt::EOS_TYPE m_EOSType;
  double m_omegaA;
  double m_omegaB;
  double m_delta1, m_delta2;

  double (CubicEoSPhaseModel::*EOS_m_function)( double );

  // Constant Properties
  std::vector< double > m_m;
  const double m_BICs;

  // Init function at instantiation
  void init();

  // Used to shorten input
  struct CubicEosMixtureCoefficients
  {
    std::vector< double > APure, BPure;
    double AMixture, BMixture;

    CubicEosMixtureCoefficients( std::size_t nComponents )
      : APure( nComponents ),
        BPure( nComponents ),
        AMixture( 0. ),
        BMixture( 0. )
    { }
  };

  CubicEosMixtureCoefficients computeMixtureCoefficients( double pressure,
                                                          double temperature,
                                                          std::vector< double > const & composition ) const;

  double computeCompressibilityFactor( double pressure,
                                       double temperature,
                                       std::vector< double > const & composition,
                                       CubicEosMixtureCoefficients const & mixCoeffs ) const;

  static double computeMoleDensity( const ComponentProperties & componentProperties,
                                    double pressure,
                                    double temperature,
                                    std::vector< double > const & composition,
                                    double Z );

  static double computeMolecularWeight( const ComponentProperties & componentProperties,
                                        std::vector< double > const & composition );

  static double computeMassDensity( double moleDensity,
                                    double mw );

  static double computeViscosity(); 
  
  std::vector< double > computeLnFugacitiesCoefficients( std::vector< double > const & composition,
                                                         double Z,
                                                         CubicEosMixtureCoefficients const & mixtureCoefficients ) const;

  static std::vector< double > solveCubicPolynomial( double m3,
                                                     double m2,
                                                     double m1,
                                                     double m0 );

  // m functions
  double m_function_PR( double omega );

  double m_function_SRK( double omega );

public:
  pvt::PHASE_TYPE getPhase() const
  {
    return m_phase;
  }

  pvt::EOS_TYPE getEosType() const
  {
    return m_EOSType;
  }
};

}
