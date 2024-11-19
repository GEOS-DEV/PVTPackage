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

#include "FactorMultiphaseSystemProperties.hpp"

namespace PVTPackage
{

FactorMultiphaseSystemProperties::FactorMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases,
                                                                    std::size_t nComponents )
  :
  m_phases( phases )
{
  for( pvt::PHASE_TYPE pt: m_phases )
  {
    m_massDensity.insert( { pt, pvt::ScalarPropertyAndDerivatives< double >( nComponents ) } );
    m_moleComposition.insert( { pt, pvt::VectorPropertyAndDerivatives< double >( nComponents, nComponents ) } );
    m_moleDensity.insert( { pt, pvt::ScalarPropertyAndDerivatives< double >( nComponents ) } );
    m_viscosity.insert( {pt, pvt::ScalarPropertyAndDerivatives< double >( nComponents ) } ); 
    m_molecularWeight.insert( { pt, pvt::ScalarPropertyAndDerivatives< double >( nComponents ) } );
    m_phaseMoleFraction.insert( { pt, pvt::ScalarPropertyAndDerivatives< double >( nComponents ) } );
  }
}

pvt::ScalarPropertyAndDerivatives< double > const & FactorMultiphaseSystemProperties::getMassDensity( pvt::PHASE_TYPE const & phase ) const
{
  return m_massDensity.at( phase );
}

pvt::VectorPropertyAndDerivatives< double > const & FactorMultiphaseSystemProperties::getMoleComposition( pvt::PHASE_TYPE const & phase ) const
{
  return m_moleComposition.at( phase );
}

pvt::ScalarPropertyAndDerivatives< double > const & FactorMultiphaseSystemProperties::getMoleDensity( pvt::PHASE_TYPE const & phase ) const
{
  return m_moleDensity.at( phase );
}

pvt::ScalarPropertyAndDerivatives< double > const & FactorMultiphaseSystemProperties::getViscosity( pvt::PHASE_TYPE const & phase ) const
{
  return m_viscosity.at( phase );
}
  
pvt::ScalarPropertyAndDerivatives< double > const & FactorMultiphaseSystemProperties::getMolecularWeight( pvt::PHASE_TYPE const & phase ) const
{
  return m_molecularWeight.at( phase );
}

pvt::ScalarPropertyAndDerivatives< double > const & FactorMultiphaseSystemProperties::getPhaseMoleFraction( pvt::PHASE_TYPE const & phase ) const
{
  return m_phaseMoleFraction.at( phase );
}

double const & FactorMultiphaseSystemProperties::getPressure() const
{
  return m_pressure;
}

const std::vector< double > & FactorMultiphaseSystemProperties::getFeed() const
{
  return m_feed;
}

void FactorMultiphaseSystemProperties::setPressure( double const & pressure )
{
  m_pressure = pressure;
}

void FactorMultiphaseSystemProperties::setFeed( std::vector< double > const & feed )
{
  m_feed = feed;
}

const std::vector< pvt::PHASE_TYPE > & FactorMultiphaseSystemProperties::getPhases() const
{
  return m_phases;
}

void FactorMultiphaseSystemProperties::setPhaseMoleFraction( pvt::PHASE_TYPE const & phase,
                                                             double const & phaseMoleFraction )
{
  m_phaseMoleFraction.at( phase ).value = phaseMoleFraction;

}

void FactorMultiphaseSystemProperties::setMoleComposition( const pvt::PHASE_TYPE & phase,
                                                           const std::vector< double > & moleComposition )
{
  m_moleComposition.at( phase ).value = moleComposition;
}

void FactorMultiphaseSystemProperties::setPhaseMoleFractionDP( pvt::PHASE_TYPE const & phase,
                                                               double const & value )
{
  m_phaseMoleFraction.at( phase ).dP = value;
}

void FactorMultiphaseSystemProperties::setMolecularWeightDP( pvt::PHASE_TYPE const & phase,
                                                             double const & value )
{
  m_molecularWeight.at( phase ).dP = value;
}

void FactorMultiphaseSystemProperties::setMoleDensityDP( pvt::PHASE_TYPE const & phase,
                                                         double const & value )
{
  m_moleDensity.at( phase ).dP = value;
}

void FactorMultiphaseSystemProperties::setMassDensityDP( pvt::PHASE_TYPE const & phase,
                                                         double const & value )
{
  m_massDensity.at( phase ).dP = value;
}

void FactorMultiphaseSystemProperties::setViscosityDP( pvt::PHASE_TYPE const & phase,
                                                       double const & value )
{
  m_viscosity.at( phase ).dP = value;
}
  
void FactorMultiphaseSystemProperties::setMoleCompositionDP( pvt::PHASE_TYPE const & phase,
                                                             std::vector< double > const & value )
{
  m_moleComposition.at( phase ).dP = value;
}

/// DZ

void FactorMultiphaseSystemProperties::setPhaseMoleFractionDZ( pvt::PHASE_TYPE const & phase,
                                                               std::size_t i,
                                                               double const & value )
{
  m_phaseMoleFraction.at( phase ).dz[i] = value;
}

void FactorMultiphaseSystemProperties::setMolecularWeightDZ( pvt::PHASE_TYPE const & phase,
                                                             std::size_t i,
                                                             double const & value )
{
  m_molecularWeight.at( phase ).dz[i] = value;
}

void FactorMultiphaseSystemProperties::setMoleDensityDZ( pvt::PHASE_TYPE const & phase,
                                                         std::size_t i,
                                                         double const & value )
{
  m_moleDensity.at( phase ).dz[i] = value;
}

void FactorMultiphaseSystemProperties::setMassDensityDZ( pvt::PHASE_TYPE const & phase,
                                                         std::size_t i,
                                                         double const & value )
{
  m_massDensity.at( phase ).dz[i] = value;
}

void FactorMultiphaseSystemProperties::setViscosityDZ( pvt::PHASE_TYPE const & phase,
                                                       std::size_t i,
                                                       double const & value )
{
  m_viscosity.at( phase ).dz[i] = value;
}

void FactorMultiphaseSystemProperties::setMoleCompositionDZ( pvt::PHASE_TYPE const & phase,
                                                             std::size_t i,
                                                             std::vector< double > const & value )
{
  // FIXME That would be more elegant with indices in the opposite order and a direct vector assignment.
  pvt::VectorPropertyAndDerivatives< double > & mcp = m_moleComposition.at( phase );
  for( std::size_t j = 0; j < mcp.value.size(); ++j )
  {
    mcp.dz[j][i] = value[j];
  }
}

}
