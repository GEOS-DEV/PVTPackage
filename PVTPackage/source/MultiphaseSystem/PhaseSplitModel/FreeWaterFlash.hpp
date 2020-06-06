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

#pragma once

#include "MultiphaseSystem/ComponentProperties.hpp"

#include "MultiphaseSystem/MultiphaseSystemProperties/FreeWaterFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

class FreeWaterFlash final : private CompositionalFlash
{
public:

  FreeWaterFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                  const std::vector< pvt::EOS_TYPE > & eosTypes,
                  ComponentProperties const & componentProperties );

  bool computeEquilibrium( FreeWaterFlashMultiphaseSystemProperties & outVariables ) const;

protected:

  std::size_t m_WaterIndex;

private:

  static bool isThreePhase( const std::vector< double > & kValues,
                            const std::vector< double > & feed,
                            const std::list< std::size_t > & nonZeroIndex,
                            double KWater_GasWater,
                            double KWater_OilWater,
                            double waterFeed,
                            std::size_t waterIndex );

  static double modifiedRachfordRiceFunction( const std::vector< double > & kValues,
                                              const std::vector< double > & feed,
                                              const std::list< std::size_t > & nonZeroIndex,
                                              double kWater_GasWater,
                                              double kWater_OilWater,
                                              double waterFeed,
                                              std::size_t waterIndex,
                                              double x );

  static double dModifiedRachfordRiceFunction_dx( const std::vector< double > & kValues,
                                                  const std::vector< double > & feed,
                                                  const std::list< std::size_t > & nonZeroIndex,
                                                  double kWater_GasWater,
                                                  double kWater_OilWater,
                                                  double waterFeed,
                                                  std::size_t waterIndex,
                                                  double x );

  static double solveModifiedRachfordRiceEquation( const std::vector< double > & kValues,
                                                   const std::vector< double > & feed,
                                                   const std::list< std::size_t > & nonZeroIndex,
                                                   double kWater_gasWater,
                                                   double kWater_oilWater,
                                                   double waterFeed,
                                                   std::size_t waterIndex );

public:
  /**
   * @brief Getter for refactor purpose
   * FIXME REFACTOR
   */
  std::size_t getWaterIndex() const
  {
    return m_WaterIndex;
  }
};

}
