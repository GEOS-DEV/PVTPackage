/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "MultiphaseSystem/CompositionalMultiphaseSystem.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "PhaseSplitModel/FreeWaterFlash.hpp"

namespace PVTPackage
{

CompositionalMultiphaseSystem::CompositionalMultiphaseSystem(std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
                                                             COMPOSITIONAL_FLASH_TYPE flash_type,
                                                             const ComponentProperties& comp_properties)
  : MultiphaseSystem(comp_properties.NComponents, phase_types)
{

  ASSERT(phase_types.size() == eos_types.size(), "The model requires as many phase models as phases");

  //Create Phase Models
  for (size_t i = 0; i != phase_types.size(); ++i)
  {
    m_MultiphaseProperties.PhaseModels[phase_types[i]] =
      std::make_shared<CubicEoSPhaseModel>(comp_properties, eos_types[i], phase_types[i]);
  }


  //Create Flash pointer
  switch (flash_type)
  {
    case COMPOSITIONAL_FLASH_TYPE::TRIVIAL:
    {
      m_Flash = new TrivialFlash(comp_properties);
      break;
    }
    case COMPOSITIONAL_FLASH_TYPE::NEGATIVE_OIL_GAS:
    {
	  ASSERT(m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::OIL) != m_MultiphaseProperties.PhaseModels.end(), "Negative two phase flash require oil phase");
	  ASSERT(m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::GAS) != m_MultiphaseProperties.PhaseModels.end(), "Negative two phase flash require gas phase");
      ASSERT(m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::LIQUID_WATER_RICH) == m_MultiphaseProperties.PhaseModels.end(), "Negative two phase flash cannot handle water phase");
      m_Flash = new NegativeTwoPhaseFlash(comp_properties);
      break;
    }
    case COMPOSITIONAL_FLASH_TYPE::FREE_WATER:
	  ASSERT(m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::OIL) != m_MultiphaseProperties.PhaseModels.end(), "Free Water flash require oil phase");
      ASSERT(m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::GAS) != m_MultiphaseProperties.PhaseModels.end(), "Free Water flash require gas phase");
	  ASSERT(m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::LIQUID_WATER_RICH) != m_MultiphaseProperties.PhaseModels.end(), "Free Watere flash require water phase");
	  m_Flash = new FreeWaterFlash(comp_properties);
	  break;
    default:
      LOGERROR("Flash type is not correct or not supported.");
  }
}

CompositionalMultiphaseSystem::~CompositionalMultiphaseSystem()
{
  delete m_Flash;
}





}


