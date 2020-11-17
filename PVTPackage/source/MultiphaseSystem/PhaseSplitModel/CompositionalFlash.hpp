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
#include "MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

#include <list>

namespace PVTPackage
{

class CompositionalFlash
{
public:

  explicit CompositionalFlash( const ComponentProperties & componentProperties );

protected:

  const ComponentProperties m_ComponentsProperties;

  static double solveRachfordRiceEquation( const std::vector< double > & kValues,
                                           const std::vector< double > & feed,
                                           const std::list< std::size_t > & nonZeroIndex );

  static std::vector< double > computeWilsonGasLiquidKvalue( const ComponentProperties & componentsProperties,
                                                             double pressure,
                                                             double temperature );

  static std::vector< double > computeWaterGasKvalue( const ComponentProperties & componentsProperties,
                                                      double pressure,
                                                      double temperature );

  static std::vector< double > computeWaterOilKvalue( const ComponentProperties & componentsProperties,
                                                      double pressure,
                                                      double temperature );

private:

  static double RachfordRiceFunction( const std::vector< double > & kValues,
                                      const std::vector< double > & feed,
                                      const std::list< std::size_t > & nonZeroIndex,
                                      double x );

  static double dRachfordRiceFunction_dx( const std::vector< double > & kValues,
                                          const std::vector< double > & feed,
                                          const std::list< std::size_t > & nonZeroIndex,
                                          double x );
};

}
