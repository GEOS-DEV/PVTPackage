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

#ifndef PVTPACKAGE_NEGATIVETWOPHASEMULTIPHASESYSTEM_HPP
#define PVTPACKAGE_NEGATIVETWOPHASEMULTIPHASESYSTEM_HPP

#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/NegativeTwoPhaseFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

#include <memory>

namespace PVTPackage
{

class NegativeTwoPhaseMultiphaseSystem : public CompositionalMultiphaseSystem
{
public:

  static std::unique_ptr< NegativeTwoPhaseMultiphaseSystem > build( const std::vector< pvt::PHASE_TYPE > & phases,
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

  NegativeTwoPhaseMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                    const std::vector< pvt::EOS_TYPE > & eosTypes,
                                    const ComponentProperties & componentProperties );

  NegativeTwoPhaseFlash m_negativeTwoPhaseFlash;

  NegativeTwoPhaseFlashMultiphaseSystemProperties m_ntpfmsp;
};

}

#endif //PVTPACKAGE_NEGATIVETWOPHASEMULTIPHASESYSTEM_HPP
