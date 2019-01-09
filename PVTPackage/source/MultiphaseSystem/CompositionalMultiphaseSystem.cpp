#include "MultiphaseSystem/CompositionalMultiphaseSystem.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "PhaseSplitModel/NegativeTwoPhaseFlash.hpp"

namespace PVTPackage
{

CompositionalMultiphaseSystem::CompositionalMultiphaseSystem(std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
                                                             COMPOSITIONAL_FLASH_TYPE flash_type,
                                                             const ComponentProperties& comp_properties)
  : MultiphaseSystem(comp_properties.NComponents, phase_types)
{

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
      m_Flash = new NegativeTwoPhaseFlash(comp_properties);
      break;
    }
    case COMPOSITIONAL_FLASH_TYPE::FREE_WATER_FLASH:
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


