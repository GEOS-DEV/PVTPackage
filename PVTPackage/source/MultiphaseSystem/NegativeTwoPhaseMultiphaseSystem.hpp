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

  const std::vector< NegativeTwoPhaseFlashMultiphaseSystemProperties > & getBatchMultiphaseSystemProperties() const
  {
    return m_batchProps;
  }
#if defined(GEOSX_USE_OPENMP)
  // Not very good, it's temporary...
  void BatchUpdate( const std::vector< double > & pressure,
                    const std::vector< double > & temperature,
                    const std::vector< std::vector< double > > & feed );
#endif
  const pvt::MultiphaseSystemProperties & getMultiphaseSystemProperties() const override;

private:

  NegativeTwoPhaseMultiphaseSystem( const std::vector< pvt::PHASE_TYPE > & phases,
                                    const std::vector< pvt::EOS_TYPE > & eosTypes,
                                    const ComponentProperties & componentProperties );

  NegativeTwoPhaseFlash m_negativeTwoPhaseFlash;

  NegativeTwoPhaseFlashMultiphaseSystemProperties m_ntpfmsp;

  std::vector< NegativeTwoPhaseFlashMultiphaseSystemProperties > m_batchProps;
};

}

#endif //PVTPACKAGE_NEGATIVETWOPHASEMULTIPHASESYSTEM_HPP
