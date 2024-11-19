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

#ifndef PVTPACKAGE_PVT_HPP
#define PVTPACKAGE_PVT_HPP

#include <memory>
#include <string>
#include <vector>

/**
 * @brief public interface to the PVT package.
 */
namespace pvt
{

/**
 * @brief Utility class used to hold scalar data and its derivatives w.r.t pressure, temperature and feed.
 * @tparam T Scalar type (double, float)
 */
template< typename T >
struct ScalarPropertyAndDerivatives
{
  /**
   * @brief Builds the data with values set to 0.
   * @param nComponents Number of fluid components.
   */
  ScalarPropertyAndDerivatives( std::size_t nComponents )
    : value( 0 ),
      dP( 0 ),
      dT( 0 ),
      dz( std::vector< double >( nComponents, 0 ) )
  { }

  /// The value of the data itself.
  T value;
  /// The derivatives w.r.t pressure.
  T dP;
  /// The derivatives w.r.t temperature.
  T dT;
  /// The derivatives w.r.t feed.
  std::vector< T > dz;
};

/**
 * @brief Utility class used to hold vector data and its derivatives w.r.t pressure, temperature and feed.
 * @tparam T Scalar type (double, float)
 */
template< typename T >
struct VectorPropertyAndDerivatives
{
   /**
    * @brief Builds the data with values set to 0.
    * @param dim Number of dimensions of the vector data.
    * @param nComponents Number of fluid components.
    */
  VectorPropertyAndDerivatives( std::size_t dim,
                                std::size_t nComponents )
    : value( std::vector< T >( dim, 0 ) ),
      dP( std::vector< T >( dim, 0 ) ),
      dT( std::vector< T >( dim, 0 ) ),
      dz( std::vector< std::vector< T > >( dim, std::vector< T >( nComponents, 0 ) ) )
  { }

  /// The value of the data itself.
  std::vector< T > value;
  /// The derivatives w.r.t pressure.
  std::vector< T > dP;
  /// The derivatives w.r.t temperature.
  std::vector< T > dT;
  /// The derivatives w.r.t feed.
  std::vector< std::vector< T > > dz;
};

enum class PHASE_TYPE : int
{
  LIQUID_WATER_RICH = 2, OIL = 1, GAS = 0, UNKNOWN = -1
};

enum class EOS_TYPE : int
{
  REDLICH_KWONG_SOAVE = 1, PENG_ROBINSON = 0, UNKNOWN = -1
};

enum class COMPOSITIONAL_FLASH_TYPE : int
{
  TRIVIAL = 0, NEGATIVE_OIL_GAS = 1, TABULATED_KVALUES = 2, FREE_WATER = 3, THREE_PHASE = 4, UNKNOWN = -1
};

/**
 * @brief Data combination for any PVT system solved.
 */
class MultiphaseSystemProperties
{
public:
  virtual ~MultiphaseSystemProperties() = default;

  /**
   * @brief Mass density data (and derivatives) for given @p phase.
   * @param phase The required phase (oil, gas, water).
   * @return The data as a ref to const.
   * @throw std::out_of_range if @p phase does not exist.
   */
  virtual const ScalarPropertyAndDerivatives< double > & getMassDensity( PHASE_TYPE const & phase ) const = 0;

  /**
   * @brief Mole composition data (and derivatives) for given @p phase.
   * @param phase The required phase (oil, gas, water).
   * @return The data as a ref to const.
   * @throw std::out_of_range if @p phase does not exist.
   */
  virtual const VectorPropertyAndDerivatives< double > & getMoleComposition( PHASE_TYPE const & phase ) const = 0;

  /**
   * @brief Mole density data (and derivatives) for given @p phase.
   * @param phase The required phase (oil, gas, water).
   * @return The data as a ref to const.
   * @throw std::out_of_range if @p phase does not exist.
   */
  virtual const ScalarPropertyAndDerivatives< double > & getMoleDensity( PHASE_TYPE const & phase ) const = 0;

  /**
   * @brief Viscosity data (and derivatives) for given @p phase.
   * @param phase The required phase (oil, gas, water).
   * @return The data as a ref to const.
   * @throw std::out_of_range if @p phase does not exist.
   */
  virtual const ScalarPropertyAndDerivatives< double > & getViscosity( PHASE_TYPE const & phase ) const = 0;

  /**
   * @brief Molecular weight data (and derivatives) for given @p phase.
   * @param phase The required phase (oil, gas, water).
   * @return The data as a ref to const.
   * @throw std::out_of_range if @p phase does not exist.
   */
  virtual const ScalarPropertyAndDerivatives< double > & getMolecularWeight( PHASE_TYPE const & phase ) const = 0;

  /**
   * @brief Phase mole fraction data (and derivatives) for given @p phase.
   * @param phase The required phase (oil, gas, water).
   * @return The data as a ref to const.
   * @throw std::out_of_range if @p phase does not exist.
   */
  virtual const ScalarPropertyAndDerivatives< double > & getPhaseMoleFraction( PHASE_TYPE const & phase ) const = 0;
};

class MultiphaseSystem
{
public:
  virtual ~MultiphaseSystem() = default;

  // TODO change to `update`
  /**
   * @brief Sets a system for given @p pressurem @p temperature and @p feed and solves it. Result is accessed through #getMultiphaseSystemProperties.
   * @param pressure
   * @param temperature
   * @param feed
   */
  virtual void Update( double pressure, double temperature, std::vector< double > feed ) = 0;
  /**
   * @brief Access the data of the system.
   * @return Reference to const datw.
   */
  virtual MultiphaseSystemProperties const & getMultiphaseSystemProperties() const = 0;
  /**
   * @brief True if the computation went OK.
   * @return A boolean.
   */
  virtual bool hasSucceeded() const = 0 ;
};

class MultiphaseSystemBuilder
{
public:

  /**
   * @brief Builds a compositional instance of a multiphase system.
   * @param flashType The type of compositional system we want (for the moment, trivial, negative two phase and free water).
   * @param phases The considered phases in the system.
   * @param eosTypes The considered equations states.
   * @param componentNames The component names
   * @param componentNames The components molar weights
   * @param componentNames The component critical temperatures
   * @param componentCriticalPressures The component critical pressures
   * @param componentOmegas The components omegas
   * @return A std::unique_ptr holding the system. The smart ptr may hold nullptr if something went wrong.
   *
   * Each same index of the component* inputs will be used as input for the same component.
   * E.g. first component will be built from componentNames[0], componentNames[0], componentNames[0]...
   */
  static std::unique_ptr< MultiphaseSystem > buildCompositional( COMPOSITIONAL_FLASH_TYPE const & flashType,
                                                                 std::vector< PHASE_TYPE > const & phases,
                                                                 std::vector< EOS_TYPE > const & eosTypes,
                                                                 std::vector< std::string > const & componentNames,
                                                                 std::vector< double > const & componentMolarWeights,
                                                                 std::vector< double > const & componentCriticalTemperatures,
                                                                 std::vector< double > const & componentCriticalPressures,
                                                                 std::vector< double > const & componentOmegas );

  /**
   * @brief Builds a live oil instance of a multiphase system.
   * @param phases The considered phases in the system.
   * @param tableFileNames The file names containing the table.
   * @param surfaceMassDensities Surface mass densities.
   * @param molarWeights molar weights.
   * @return A std::unique_ptr holding the system. The smart ptr may hold nullptr if something went wrong.
   */
  static std::unique_ptr< MultiphaseSystem > buildLiveOil( const std::vector< pvt::PHASE_TYPE > & phases,
                                                           const std::vector< std::string > & tableFileNames,
                                                           const std::vector< double > & surfaceMassDensities,
                                                           const std::vector< double > & molarWeights );

  /**
   * @brief Builds a dead oil instance of a multiphase system.
   * @param phases The considered phases in the system.
   * @param tableFileNames The file names containing the table.
   * @param surfaceMassDensities Surface mass densities.
   * @param molarWeights molar weights.
   * @return A std::unique_ptr holding the system. The smart ptr may hold nullptr if something went wrong.
   */
  static std::unique_ptr< MultiphaseSystem > buildDeadOil( const std::vector< pvt::PHASE_TYPE > & phases,
                                                           const std::vector< std::string > & tableFileNames,
                                                           const std::vector< double > & surfaceMassDensities,
                                                           const std::vector< double > & molarWeights );
};

}

#endif //PVTPACKAGE_PVT_HPP
