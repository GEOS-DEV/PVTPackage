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

#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

#include "pvt/pvt.hpp"

#include <list>

namespace PVTPackage
{

class CompositionalFlash
{
public:

  CompositionalFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                      const std::vector< pvt::EOS_TYPE > & eosTypes,
                      ComponentProperties const & componentProperties );

protected:

  static double solveRachfordRiceEquation( const std::vector< double > & kValues,
                                           const std::vector< double > & feed,
                                           const std::list< std::size_t > & nonZeroIndex );

  // It may be possible to redefine these static functions as members in order to hide componentsProperties arg.
  std::vector< double > computeWilsonGasLiquidKvalue( double pressure,
                                                      double temperature ) const;

  std::vector< double > computeWaterGasKvalue( double pressure,
                                               double temperature ) const;

  std::vector< double > computeWaterOilKvalue( double pressure,
                                               double temperature ) const;

  const CubicEoSPhaseModel & getCubicEoSPhaseModel( const pvt::PHASE_TYPE & phase ) const;

  std::size_t getNComponents() const;

  std::size_t getWaterIndex() const;

private:

  std::map< pvt::PHASE_TYPE, CubicEoSPhaseModel > m_phaseModels;

  const ComponentProperties m_componentProperties;

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
