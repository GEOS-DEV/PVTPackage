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

#pragma once

#include "MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/FactorMultiphaseSystemProperties.hpp"

#include "Utils/math.hpp"

#include "pvt/pvt.hpp"

#include <math.h>
#include <limits>
#include <vector>

namespace PVTPackage
{

class MultiphaseSystem : public pvt::MultiphaseSystem
{
public:

  bool hasSucceeded() const final;

protected:

  enum class State
  {
    NOT_INITIALIZED, SUCCESS, NOT_CONVERGED, FATAL_ERROR, NOT_IMPLEMENTED
  };

  /// Success indicator for system state update
  State m_stateIndicator;

  /**
   * @brief Computes the equilibrium and some derivatives for given @p flash.
   * @tparam F The flash type (F stands for flash).
   * @tparam MSP The MultiphaseSystemProperties type.
   * @param flash The flash instance.
   * @param properties The data the flash algorithm will be using.
   * @return True in case of success.
   *
   * @note This function computes the derivatives w.r.t. pressure, components. Not temperature.
   */
  template< class F, class MSP >
  static bool computeEquilibriumAndDerivativesNoTemperature( const F & flash,
                                                             MSP & properties )
  {
    bool success = flash.computeEquilibrium( properties );

    double const & pressure = properties.getPressure();

    double const sqrtPrecision = sqrt( std::numeric_limits< double >::epsilon() );

    // Copying for finite difference process
    MSP pEps = properties;

    // Pressure
    {
      const double dPressure = sqrtPrecision * ( std::fabs( pressure ) + sqrtPrecision );
      pEps.setPressure( pressure + dPressure );
      success &= flash.computeEquilibrium( pEps );
      updateDerivativeDPFiniteDifference( properties, pEps, dPressure );
      pEps.setPressure( pressure );
    }

    // Feed
    {
      std::vector< double > const & savedFeed = properties.getFeed();
      for( std::size_t iComponent = 0; iComponent < savedFeed.size(); ++iComponent )
      {
        double dz = sqrtPrecision * ( std::fabs( savedFeed[iComponent] ) + sqrtPrecision );
        if( savedFeed[iComponent] + dz > 1 )
        {
          dz = -dz;
        }
        std::vector< double > newFeed( savedFeed );
        newFeed[iComponent] += dz;
        pEps.setFeed( math::Normalize( newFeed ) );
        success &= flash.computeEquilibrium( pEps );
        updateDerivativeDZFiniteDifference( iComponent, properties, pEps, dz );
        pEps.setFeed( savedFeed );
      }
    }

    return success;
  }

private:

  static void updateDerivativeDPFiniteDifference( FactorMultiphaseSystemProperties & sysProps,
                                                  const FactorMultiphaseSystemProperties & perturbedSysProps,
                                                  double dPressure );

  static void updateDerivativeDZFiniteDifference( std::size_t iComponent,
                                                  FactorMultiphaseSystemProperties & sysProps,
                                                  const FactorMultiphaseSystemProperties & perturbedSysProps,
                                                  double dz );
};

class CompositionalMultiphaseSystem : public MultiphaseSystem
{
protected:

  static bool areComponentDataConsistent( std::vector< std::string > const & componentNames,
                                          std::vector< double > const & componentMolarWeights,
                                          std::vector< double > const & componentCriticalTemperatures,
                                          std::vector< double > const & componentCriticalPressures,
                                          std::vector< double > const & componentOmegas );

  /**
   * @brief Computes the equilibrium and derivatives for given @p flash.
   * @tparam F The flash type (F stands for flash).
   * @tparam MSP The MultiphaseSystemProperties type.
   * @param flash The flash instance.
   * @param properties The data the flash algorithm will be using.
   * @return True in case of success.
   *
   * @note This function computes the derivatives w.r.t. pressure, temperature, components.
   */
  template< class F, class MSP >
  static bool computeEquilibriumAndDerivativesWithTemperature( const F & flash,
                                                               MSP & properties )
  {
    bool success = computeEquilibriumAndDerivativesNoTemperature( flash, properties );

    double const sqrtPrecision = sqrt( std::numeric_limits< double >::epsilon() );

    // Copying for finite difference process
    MSP pEps = properties;

    // Temperature finite difference process
    double const & temperature = properties.getTemperature();
    const double dTemperature = sqrtPrecision * ( std::fabs( temperature ) + sqrtPrecision );
    pEps.setTemperature( temperature + dTemperature );
    success &= flash.computeEquilibrium( pEps );
    updateDerivativeDTFiniteDifference( properties, pEps, dTemperature );
    pEps.setTemperature( temperature );

    return success;
  }

private:

  static void updateDerivativeDTFiniteDifference( CompositionalMultiphaseSystemProperties & sysProps,
                                                  const CompositionalMultiphaseSystemProperties & perturbedSysProps,
                                                  double dTemperature );
};

/**
 * @brief Utility class containing table reading functions.
 *
 * @note A class is made just to reduce visibility.
 */
class TableReader
{
private:

  static void readTable( const std::string & fileName,
                         std::vector< std::vector< double > > & data,
                         unsigned int minRowLen );

  static std::vector< std::vector< std::vector< double > > > readTables( const std::vector< pvt::PHASE_TYPE > & phases,
                                                                         const std::vector< std::string > & tableFileNames );

protected:

  struct Properties
  {
    std::vector< std::vector< double > > oilTable;
    double oilSurfaceMassDensity;
    double oilSurfaceMolecularWeight;
    std::vector< std::vector< double > > gasTable;
    double gasSurfaceMassDensity;
    double gasSurfaceMolecularWeight;
    std::vector< double > waterTable;
    double waterSurfaceMassDensity;
    double waterSurfaceMolecularWeight;
  };

  static Properties buildTables( const std::vector< pvt::PHASE_TYPE > & phases,
                                 const std::vector< std::string > & tableFileNames,
                                 const std::vector< double > & surfaceMassDensities,
                                 const std::vector< double > & molarWeights );
};

}
