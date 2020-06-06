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

#include "CompositionalMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

CompositionalMultiphaseSystemProperties::CompositionalMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                                  std::size_t nComponents )
  :
  FactorMultiphaseSystemProperties( phases, nComponents )
{
  for( pvt::PHASE_TYPE pt: phases )
  {
    // FIXME not so sure that all models use those two following data.
    m_lnFugacity.insert( { pt, std::vector< double >( nComponents, 0. ) } );
    m_compressibilityFactor.insert( { pt, 0. } );
  }
}

double const & CompositionalMultiphaseSystemProperties::getTemperature() const
{
  return m_temperature;
}

void CompositionalMultiphaseSystemProperties::setTemperature( double const & temperature )
{
  m_temperature = temperature;
}

void CompositionalMultiphaseSystemProperties::setOilMoleComposition( std::vector< double > const & moleComposition )
{
  setMoleComposition( pvt::PHASE_TYPE::OIL, moleComposition );
}

void CompositionalMultiphaseSystemProperties::setGasMoleComposition( std::vector< double > const & moleComposition )
{
  setMoleComposition( pvt::PHASE_TYPE::GAS, moleComposition );
}

void CompositionalMultiphaseSystemProperties::setOilFraction( double const & fraction )
{
  setPhaseMoleFraction( pvt::PHASE_TYPE::OIL, fraction );
}

void CompositionalMultiphaseSystemProperties::setGasFraction( double const & fraction )
{
  setPhaseMoleFraction( pvt::PHASE_TYPE::GAS, fraction );
}

void CompositionalMultiphaseSystemProperties::setModelProperties( const pvt::PHASE_TYPE & phase,
                                                                  CubicEoSPhaseModel::Properties const & properties )
{
  m_massDensity.at( phase ).value = properties.massDensity;
  m_moleDensity.at( phase ).value = properties.moleDensity;
  m_molecularWeight.at( phase ).value = properties.molecularWeight;
  // FIXME not so sure that all models use those two following data.
  m_compressibilityFactor.at( phase ) = properties.compressibilityFactor;
  m_lnFugacity.at( phase ) = properties.lnFugacityCoefficients;
}

/// DT

void CompositionalMultiphaseSystemProperties::setPhaseMoleFractionDT( pvt::PHASE_TYPE const & phase,
                                                                      double const & value )
{
  m_phaseMoleFraction.at( phase ).dT = value;
}

void CompositionalMultiphaseSystemProperties::setMolecularWeightDT( pvt::PHASE_TYPE const & phase,
                                                                    double const & value )
{
  m_molecularWeight.at( phase ).dT = value;
}

void CompositionalMultiphaseSystemProperties::setMoleDensityDT( pvt::PHASE_TYPE const & phase,
                                                                double const & value )
{
  m_moleDensity.at( phase ).dT = value;
}

void CompositionalMultiphaseSystemProperties::setMassDensityDT( pvt::PHASE_TYPE const & phase,
                                                                double const & value )
{
  m_massDensity.at( phase ).dT = value;
}

void CompositionalMultiphaseSystemProperties::setMoleCompositionDT( pvt::PHASE_TYPE const & phase,
                                                                    std::vector< double > const & value )
{
  m_moleComposition.at( phase ).dT = value;
}

}