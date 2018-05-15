#include "MultiphaseSystem/CompositionalMultiphaseSystem.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"

namespace PVTPackage
{
	CompositionalMultiphaseSystem::CompositionalMultiphaseSystem( std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
	                                               COMPOSITIONAL_FLASH_TYPE flash_type,
	                                               ComponentProperties comp_properties):
		MultiphaseSystem(comp_properties.NComponents, phase_types), m_ComponentProperties(std::move(comp_properties)),
		m_FlashType(flash_type),
		m_CompositionalFlash(nullptr)
	{
		//Assign phase type to eos models
		ASSERT(phase_types.size() == eos_types.size(), "EOS types and Phase type must be same dimension.");
		for (size_t i = 0; i != eos_types.size(); ++i)
		{
			m_EoSTypes[phase_types[i]] = eos_types[i];
		}

		//Create Phase Models
		for (size_t i = 0; i != m_Phases.size(); ++i)
		{
			m_PhaseModel[m_Phases[i]] = new CubicEoSPhase(&m_ComponentProperties, m_EoSTypes[m_Phases[i]], m_Phases[i]);
		}


		//Create Flash pointer
		switch (m_FlashType)
		{
		case COMPOSITIONAL_FLASH_TYPE::TRIVIAL:
			{
				m_CompositionalFlash = new TrivialFlash(phase_types, eos_types, &m_ComponentProperties, &m_PhaseModel);
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
		delete m_CompositionalFlash;
	}

	void CompositionalMultiphaseSystem::Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables)
	{
		m_CompositionalFlash->ComputeEquilibrium(pressure,temperature,feed, out_variables);
	}

}


