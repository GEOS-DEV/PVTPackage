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

#include "MultiphaseSystem/MultiphaseSystem.hpp"

#include "Utils/FileUtils.hpp"

#include <algorithm>

namespace PVTPackage
{

bool MultiphaseSystem::hasSucceeded() const
{
  return m_stateIndicator == State::SUCCESS;
}

/**
 * @brief Computes the derivatives through finite difference process
 * @tparam C Type of the consumer.
 * @param reference Reference solution.
 * @param variation Variation solution.
 * @param delta Finite difference step.
 * @param consumer Callback that will be called with the computed derivatives.
 *
 * Generic functions (pure code factorisation) that evaluates the derivatives
 * through finite differences process between reference @p sysProps and variation @p diffed.
 * This function then calls @p consumer, which role should mainly to store the computed data where they should be.
 */
template< class C >
void updateDerivativeFiniteDifference( const FactorMultiphaseSystemProperties & reference,
                                       const FactorMultiphaseSystemProperties & variation,
                                       double delta,
                                       const C & consumer )
{
  for( const pvt::PHASE_TYPE & phase : reference.getPhases() )
  {
    const double dPhaseMoleFraction = ( variation.getPhaseMoleFraction( phase ).value - reference.getPhaseMoleFraction( phase ).value ) / delta;
    const double dMolecularWeight = ( variation.getMolecularWeight( phase ).value - reference.getMolecularWeight( phase ).value ) / delta;
    const double dMoleDensity = ( variation.getMoleDensity( phase ).value - reference.getMoleDensity( phase ).value ) / delta;
    const double dMassDensity = ( variation.getMassDensity( phase ).value - reference.getMassDensity( phase ).value ) / delta;

    std::vector< double > dMoleComposition;
    {
      const std::vector< double > & moleComposition = reference.getMoleComposition( phase ).value;
      const std::vector< double > & diffedMoleComposition = variation.getMoleComposition( phase ).value;
      dMoleComposition.resize( moleComposition.size() );
      for( std::size_t i = 0; i < moleComposition.size(); ++i )
      {
        dMoleComposition[i] = ( diffedMoleComposition[i] - moleComposition[i] ) / delta;
      }
    }

    consumer( phase, dPhaseMoleFraction, dMolecularWeight, dMoleDensity, dMassDensity, dMoleComposition );
  }
}

void MultiphaseSystem::updateDerivativeDPFiniteDifference( FactorMultiphaseSystemProperties & sysProps,
                                                           const FactorMultiphaseSystemProperties & diffed,
                                                           double dPressure )
{
  auto callBack = [&sysProps]( pvt::PHASE_TYPE phase,
                               double dPhaseMoleFraction,
                               double dMolecularWeight,
                               double dMoleDensity,
                               double dMassDensity,
                               const std::vector< double > & dMoleComposition )
  {
    sysProps.setPhaseMoleFractionDP( phase, dPhaseMoleFraction );
    sysProps.setMolecularWeightDP( phase, dMolecularWeight );
    sysProps.setMoleDensityDP( phase, dMoleDensity );
    sysProps.setMassDensityDP( phase, dMassDensity );
    sysProps.setMoleCompositionDP( phase, dMoleComposition );
  };

  updateDerivativeFiniteDifference( sysProps, diffed, dPressure, callBack );
}

void MultiphaseSystem::updateDerivativeDZFiniteDifference( std::size_t iComponent,
                                                           FactorMultiphaseSystemProperties & sysProps,
                                                           FactorMultiphaseSystemProperties const & diffed,
                                                           double dz )
{
  for( const pvt::PHASE_TYPE & phase : sysProps.getPhases() )
  {
    {
      const double tmp = ( diffed.getPhaseMoleFraction( phase ).value - sysProps.getPhaseMoleFraction( phase ).value ) / dz;
      sysProps.setPhaseMoleFractionDZ( phase, iComponent, tmp );
    }
    {
      const double tmp = ( diffed.getMolecularWeight( phase ).value - sysProps.getMolecularWeight( phase ).value ) / dz;
      sysProps.setMolecularWeightDZ( phase, iComponent, tmp );
    }
    {
      double const tmp = ( diffed.getMoleDensity( phase ).value - sysProps.getMoleDensity( phase ).value ) / dz;
      sysProps.setMoleDensityDZ( phase, iComponent, tmp );
    }
    {
      double const tmp = ( diffed.getMassDensity( phase ).value - sysProps.getMassDensity( phase ).value ) / dz;
      sysProps.setMassDensityDZ( phase, iComponent, tmp );
    }
    {
      const std::size_t nComponents = sysProps.getFeed().size(); // FIXME add a nComponents getter...
      std::vector< double > dMole( nComponents );
      for( std::size_t i = 0; i < nComponents; ++i )
      {
        dMole[i] = ( diffed.getMoleComposition( phase ).value[i] - sysProps.getMoleComposition( phase ).value[i] ) / dz;
      }
      sysProps.setMoleCompositionDZ( phase, iComponent, dMole );
    }
  }
}

void TableReader::readTable( std::string const & fileName,
                             std::vector< std::vector< double>> & data,
                             unsigned int minRowLen )
{
  FileUtils::ReadTable( fileName, data );

  for( const auto & row : data )
  {
    ASSERT( row.size() >= minRowLen, "Too few entries in a row of table " + fileName
                                     + ", minimum " + std::to_string( minRowLen ) + " required" );
  }
}

std::vector< std::vector< std::vector< double > > > TableReader::readTables( std::vector< pvt::PHASE_TYPE > const & phases,
                                                                             std::vector< std::string > const & tableFileNames )
{
  const std::size_t nPhases = phases.size();
  std::vector< std::vector< std::vector< double > > > result( nPhases );

  for( unsigned i = 0; i < nPhases; ++i )
  {
    const int rowMinLen = ( phases[i] == pvt::PHASE_TYPE::LIQUID_WATER_RICH ) ? 4 : 3;
    readTable( tableFileNames[i], result[i], rowMinLen );
  }

  return result;
}

TableReader::Properties TableReader::buildTables( const std::vector< pvt::PHASE_TYPE > & phases,
                                                  const std::vector< std::string > & tableFileNames,
                                                  const std::vector< double > & surfaceDensities,
                                                  const std::vector< double > & molarWeights )
{
  // Check if both oil and gas are defined
  const bool containsOil = std::find( phases.cbegin(), phases.cend(), pvt::PHASE_TYPE::OIL ) != phases.end();
  const bool containsGas = std::find( phases.cbegin(), phases.cend(), pvt::PHASE_TYPE::GAS ) != phases.end();
  ASSERT( containsOil and containsGas, "Both oil and gas phase must be defined for neither DeadOil nor BlackOil model" );

  // Reading data from files
  const std::vector< std::vector< std::vector< double > > > phaseTables = readTables( phases, tableFileNames );

  Properties result;

  for( std::size_t i = 0; i != phases.size(); ++i )
  {
    pvt::PHASE_TYPE const & phase = phases[i];

    std::vector< std::vector< double > > const & phaseTable = phaseTables[i];
    double const & surfaceDensity = surfaceDensities[i];
    double const & molarWeight = molarWeights[i];

    switch( phase )
    {
      case pvt::PHASE_TYPE::OIL:
        result.oilTable = phaseTable;
        result.oilSurfaceMassDensity = surfaceDensity;
        result.oilSurfaceMolecularWeight = molarWeight;
        break;
      case pvt::PHASE_TYPE::GAS:
        result.gasTable = phaseTable;
        result.gasSurfaceMassDensity = surfaceDensity;
        result.gasSurfaceMolecularWeight = molarWeight;
        break;
      case pvt::PHASE_TYPE::LIQUID_WATER_RICH:
        ASSERT( phaseTable.size() == 1, "Too many lines in water properties table" );
        result.waterTable = phaseTable[0];
        result.waterSurfaceMassDensity = surfaceDensity;
        result.waterSurfaceMolecularWeight = molarWeight;
        break;
      default:
        LOGERROR( "Phase type not supported for Black Oil model" );
    }
  }

  return result;
}

bool CompositionalMultiphaseSystem::areComponentDataConsistent( std::vector< std::string > const & componentNames,
                                                                std::vector< double > const & componentMolarWeights,
                                                                std::vector< double > const & componentCriticalTemperatures,
                                                                std::vector< double > const & componentCriticalPressures,
                                                                std::vector< double > const & componentOmegas )
{
  const std::size_t nComponents = componentNames.size();
  return componentMolarWeights.size() == nComponents &&
         componentCriticalTemperatures.size() == nComponents &&
         componentCriticalPressures.size() == nComponents &&
         componentOmegas.size() == nComponents;
}

void CompositionalMultiphaseSystem::updateDerivativeDTFiniteDifference( CompositionalMultiphaseSystemProperties & sysProps,
                                                                        const CompositionalMultiphaseSystemProperties & diffed,
                                                                        double dTemperature )
{
  auto callBack = [&sysProps]( pvt::PHASE_TYPE phase,
                               double dPhaseMoleFraction,
                               double dMolecularWeight,
                               double dMoleDensity,
                               double dMassDensity,
                               const std::vector< double > & dMoleComposition )
  {
    sysProps.setPhaseMoleFractionDT( phase, dPhaseMoleFraction );
    sysProps.setMolecularWeightDT( phase, dMolecularWeight );
    sysProps.setMoleDensityDT( phase, dMoleDensity );
    sysProps.setMassDensityDT( phase, dMassDensity );
    sysProps.setMoleCompositionDT( phase, dMoleComposition );
  };

  updateDerivativeFiniteDifference( sysProps, diffed, dTemperature, callBack );
}

}
