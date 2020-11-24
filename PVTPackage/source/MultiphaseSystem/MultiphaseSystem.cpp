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

struct FiniteDifferenceDerivatives
{
  double dPhaseMoleFraction;
  double dMolecularWeight;
  double dMoleDensity;
  double dMassDensity;
  double dViscosity;
  std::vector< double > dMoleComposition;
};

/**
 * @brief Computes the derivatives through finite difference process
 * @param phase The phase for which we compute the finite difference
 * @param reference Reference solution.
 * @param variation Variation solution.
 * @param delta Finite difference step.
 * @return A structure holding the finite difference computed values.
 *
 * Generic functions (pure code factorisation) that evaluates the derivatives
 * through finite differences process between @p reference and @p variation.
 */
FiniteDifferenceDerivatives updateDerivativeFiniteDifference( const pvt::PHASE_TYPE & phase,
                                                              const FactorMultiphaseSystemProperties & reference,
                                                              const FactorMultiphaseSystemProperties & variation,
                                                              double delta )
{
  FiniteDifferenceDerivatives result;

  result.dPhaseMoleFraction = ( variation.getPhaseMoleFraction( phase ).value - reference.getPhaseMoleFraction( phase ).value ) / delta;
  result.dMolecularWeight = ( variation.getMolecularWeight( phase ).value - reference.getMolecularWeight( phase ).value ) / delta;
  result.dMoleDensity = ( variation.getMoleDensity( phase ).value - reference.getMoleDensity( phase ).value ) / delta;
  result.dMassDensity = ( variation.getMassDensity( phase ).value - reference.getMassDensity( phase ).value ) / delta;
  result.dViscosity = ( variation.getViscosity( phase ).value - reference.getViscosity( phase ).value ) / delta;  

  const std::vector< double > & refMoleComposition = reference.getMoleComposition( phase ).value;
  const std::vector< double > & varMoleComposition = variation.getMoleComposition( phase ).value;
  result.dMoleComposition.resize( refMoleComposition.size() );
  for( std::size_t i = 0; i < refMoleComposition.size(); ++i )
  {
    result.dMoleComposition[i] = ( varMoleComposition[i] - refMoleComposition[i] ) / delta;
  }

  return result;
}

void MultiphaseSystem::updateDerivativeDPFiniteDifference( FactorMultiphaseSystemProperties & sysProps,
                                                           const FactorMultiphaseSystemProperties & perturbedSysProps,
                                                           double dPressure )
{
  for( const pvt::PHASE_TYPE & phase: sysProps.getPhases() )
  {
    const FiniteDifferenceDerivatives values = updateDerivativeFiniteDifference( phase, sysProps, perturbedSysProps, dPressure );

    sysProps.setPhaseMoleFractionDP( phase, values.dPhaseMoleFraction );
    sysProps.setMolecularWeightDP( phase, values.dMolecularWeight );
    sysProps.setMoleDensityDP( phase, values.dMoleDensity );
    sysProps.setMassDensityDP( phase, values.dMassDensity );
    sysProps.setViscosityDP( phase, values.dViscosity );
    sysProps.setMoleCompositionDP( phase, values.dMoleComposition );
  }
}

void MultiphaseSystem::updateDerivativeDZFiniteDifference( std::size_t iComponent,
                                                           FactorMultiphaseSystemProperties & sysProps,
                                                           FactorMultiphaseSystemProperties const & perturbedSysProps,
                                                           double dz )
{
  for( const pvt::PHASE_TYPE & phase: sysProps.getPhases() )
  {
    const FiniteDifferenceDerivatives values = updateDerivativeFiniteDifference( phase, sysProps, perturbedSysProps, dz );

    sysProps.setPhaseMoleFractionDZ( phase, iComponent, values.dPhaseMoleFraction );
    sysProps.setMolecularWeightDZ( phase, iComponent, values.dMolecularWeight );
    sysProps.setMoleDensityDZ( phase, iComponent, values.dMoleDensity );
    sysProps.setMassDensityDZ( phase, iComponent, values.dMassDensity );
    sysProps.setViscosityDZ( phase, iComponent, values.dViscosity );
    sysProps.setMoleCompositionDZ( phase, iComponent, values.dMoleComposition );
  }
}

void TableReader::readTable( std::string const & fileName,
                             std::vector< std::vector< double > > & data,
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
                                                  const std::vector< double > & surfaceMassDensities,
                                                  const std::vector< double > & molarWeights )
{
  // Check if both oil and gas are defined
  const bool containsOil = std::find( phases.cbegin(), phases.cend(), pvt::PHASE_TYPE::OIL ) != phases.end();
  const bool containsGas = std::find( phases.cbegin(), phases.cend(), pvt::PHASE_TYPE::GAS ) != phases.end();
  ASSERT( containsOil or containsGas, "Both oil and gas phase must be defined for neither DeadOil nor BlackOil model" );

  // Reading data from files
  const std::vector< std::vector< std::vector< double > > > phaseTables = readTables( phases, tableFileNames );

  Properties result;
  // initialize result, in case the water or gas tables are not provided (for the two-phase case)
  result.oilSurfaceMassDensity       = 0.0;
  result.oilSurfaceMolecularWeight   = 0.0;
  result.gasSurfaceMassDensity       = 0.0;
  result.gasSurfaceMolecularWeight   = 0.0;
  result.waterSurfaceMassDensity     = 0.0;
  result.waterSurfaceMolecularWeight = 0.0;
  
  for( std::size_t i = 0; i != phases.size(); ++i )
  {
    pvt::PHASE_TYPE const & phase = phases[i];

    std::vector< std::vector< double > > const & phaseTable = phaseTables[i];
    double const & surfaceMassDensity = surfaceMassDensities[i];
    double const & molarWeight = molarWeights[i];

    switch( phase )
    {
      case pvt::PHASE_TYPE::OIL:
        result.oilTable = phaseTable;
        result.oilSurfaceMassDensity = surfaceMassDensity;
        result.oilSurfaceMolecularWeight = molarWeight;
        break;
      case pvt::PHASE_TYPE::GAS:
        result.gasTable = phaseTable;
        result.gasSurfaceMassDensity = surfaceMassDensity;
        result.gasSurfaceMolecularWeight = molarWeight;
        break;
      case pvt::PHASE_TYPE::LIQUID_WATER_RICH:
        ASSERT( phaseTable.size() == 1, "Too many lines in water properties table" );
        result.waterTable = phaseTable[0];
        result.waterSurfaceMassDensity = surfaceMassDensity;
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
                                                                        const CompositionalMultiphaseSystemProperties & perturbedSysProps,
                                                                        double dTemperature )
{
  for( const pvt::PHASE_TYPE & phase: sysProps.getPhases() )
  {
    const FiniteDifferenceDerivatives values = updateDerivativeFiniteDifference( phase, sysProps, perturbedSysProps, dTemperature );

    sysProps.setPhaseMoleFractionDT( phase, values.dPhaseMoleFraction );
    sysProps.setMolecularWeightDT( phase, values.dMolecularWeight );
    sysProps.setMoleDensityDT( phase, values.dMoleDensity );
    sysProps.setMassDensityDT( phase, values.dMassDensity );
    sysProps.setViscosityDT( phase, values.dViscosity );
    sysProps.setMoleCompositionDT( phase, values.dMoleComposition );
  }
}

}
