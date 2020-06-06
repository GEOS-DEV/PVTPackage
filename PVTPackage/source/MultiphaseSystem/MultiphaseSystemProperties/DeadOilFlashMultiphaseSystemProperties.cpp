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

#include "DeadOilFlashMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

#include <cmath>

namespace PVTPackage
{

DeadOilFlashMultiphaseSystemProperties::DeadOilFlashMultiphaseSystemProperties( std::size_t nComponents,
                                                                                std::vector< std::vector< double > > const & PVDO,
                                                                                double oilSurfaceMassDensity,
                                                                                double oilSurfaceMolecularWeight,
                                                                                std::vector< std::vector< double > > const & PVDG,
                                                                                double gasSurfaceMassDensity,
                                                                                double gasSurfaceMolecularWeight,
                                                                                std::vector< double > const & PVTW,
                                                                                double waterSurfaceMassDensity,
                                                                                double waterSurfaceMolecularWeight )
  :
  BlackOilDeadOilMultiphaseSystemProperties( nComponents ),
  m_oilPhaseModel( pvt::PHASE_TYPE::OIL, PVDO, oilSurfaceMassDensity, oilSurfaceMolecularWeight ),
  m_gasPhaseModel( pvt::PHASE_TYPE::GAS, PVDG, gasSurfaceMassDensity, gasSurfaceMolecularWeight ),
  m_waterPhaseModel( PVTW, waterSurfaceMassDensity, waterSurfaceMolecularWeight )
{
  for( pvt::PHASE_TYPE pt: getPhases() )
  {
    m_lnFugacity.insert( { pt, pvt::VectorPropertyAndDerivatives< double >( nComponents, nComponents ) } );
  }

  m_moleComposition.at( pvt::PHASE_TYPE::OIL ).value = { 1., 0., 0. };
  m_moleComposition.at( pvt::PHASE_TYPE::GAS ).value = { 0., 1., 0. };
  m_moleComposition.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { 0., 0., 1. };

  m_lnFugacity.at( pvt::PHASE_TYPE::OIL ).value = { std::log( 1. ), std::log( 1. ), std::log( 1. ) };
  m_lnFugacity.at( pvt::PHASE_TYPE::GAS ).value = { std::log( 1. ), std::log( 1. ), std::log( 1. ) };
  m_lnFugacity.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = { std::log( 1. ), std::log( 1. ), std::log( 1. ) };
}

double DeadOilFlashMultiphaseSystemProperties::getOilPhaseMoleFraction() const
{
  return m_phaseMoleFraction.at( pvt::PHASE_TYPE::OIL ).value;
}

double DeadOilFlashMultiphaseSystemProperties::getGasPhaseMoleFraction() const
{
  return m_phaseMoleFraction.at( pvt::PHASE_TYPE::GAS ).value;
}

double DeadOilFlashMultiphaseSystemProperties::getWaterPhaseMoleFraction() const
{
  return m_phaseMoleFraction.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value;
}

DeadOil_PhaseModel const & DeadOilFlashMultiphaseSystemProperties::getOilPhaseModel() const
{
  return m_oilPhaseModel;
}

DeadOil_PhaseModel const & DeadOilFlashMultiphaseSystemProperties::getGasPhaseModel() const
{
  return m_gasPhaseModel;
}

BlackOil_WaterModel const & DeadOilFlashMultiphaseSystemProperties::getWaterPhaseModel() const
{
  return m_waterPhaseModel;
}

void DeadOilFlashMultiphaseSystemProperties::setFeed( std::vector< double > const & feed )
{
  FactorMultiphaseSystemProperties::setFeed( feed );

  // FIXME Hard coded indices, use constants
  m_phaseMoleFraction.at( pvt::PHASE_TYPE::OIL ).value = feed[0];
  m_phaseMoleFraction.at( pvt::PHASE_TYPE::GAS ).value = feed[1];
  m_phaseMoleFraction.at( pvt::PHASE_TYPE::LIQUID_WATER_RICH ).value = feed[2];
}

}
