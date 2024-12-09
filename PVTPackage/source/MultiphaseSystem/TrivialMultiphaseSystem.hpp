/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 TotalEnergies
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#ifndef PVTPACKAGE_TRIVIALMULTIPHASESYSTEM_HPP
#define PVTPACKAGE_TRIVIALMULTIPHASESYSTEM_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/TrivialFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

#include <memory>
#include <string>
#include <vector>

namespace PVTPackage
{

class TrivialMultiphaseSystem : public CompositionalMultiphaseSystem
{
public:

  static std::unique_ptr< TrivialMultiphaseSystem > build( std::vector< pvt::PHASE_TYPE > const & phases,
                                                           std::vector< pvt::EOS_TYPE > const & eosTypes,
                                                           std::vector< std::string > const & componentNames,
                                                           std::vector< double > const & componentMolarWeights,
                                                           std::vector< double > const & componentCriticalTemperatures,
                                                           std::vector< double > const & componentCriticalPressures,
                                                           std::vector< double > const & componentOmegas );

  virtual void Update( double pressure, double temperature, std::vector< double > feed ) override;

  const pvt::MultiphaseSystemProperties & getMultiphaseSystemProperties() const override;

private:

  TrivialMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                           const std::vector< pvt::EOS_TYPE > & eosTypes,
                           const ComponentProperties & componentProperties );

  TrivialFlash m_trivialFlash;

  TrivialFlashMultiphaseSystemProperties m_tfmsp;
};

}

#endif //PVTPACKAGE_TRIVIALMULTIPHASESYSTEM_HPP
