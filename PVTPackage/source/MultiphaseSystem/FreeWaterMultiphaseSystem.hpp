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

#ifndef PVTPACKAGE_FREEWATERMULTIPHASESYSTEM_HPP
#define PVTPACKAGE_FREEWATERMULTIPHASESYSTEM_HPP

#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/FreeWaterFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/FreeWaterFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

class FreeWaterMultiphaseSystem : public CompositionalMultiphaseSystem
{
public:

  static std::unique_ptr< FreeWaterMultiphaseSystem > build( const std::vector< pvt::PHASE_TYPE > & phases,
                                                             const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                             const std::vector< std::string > & componentNames,
                                                             const std::vector< double > & componentMolarWeights,
                                                             const std::vector< double > & componentCriticalTemperatures,
                                                             const std::vector< double > & componentCriticalPressures,
                                                             const std::vector< double > & componentOmegas );

  void Update( double pressure,
               double temperature,
               std::vector< double > feed ) override;

  const pvt::MultiphaseSystemProperties & getMultiphaseSystemProperties() const override;

private:

  FreeWaterMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                             const std::vector< pvt::EOS_TYPE > & eosTypes,
                             const ComponentProperties & componentProperties );

  FreeWaterFlash m_freeWaterFlash;

  FreeWaterFlashMultiphaseSystemProperties m_fwfmsp;
};

}

#endif //PVTPACKAGE_FREEWATERMULTIPHASESYSTEM_HPP
