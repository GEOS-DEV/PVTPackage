#include "MultiphaseSystem/CompositionalMultiphaseSystem.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{
	CompositionalMultiphaseSystem::CompositionalMultiphaseSystem( std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
	                                               COMPOSITIONAL_FLASH_TYPE flash_type,
	                                               ComponentProperties comp_properties):
		MultiphaseSystem(comp_properties.NComponents, phase_types.size()),
		m_CompositionalFlash(nullptr)
	{
		
		//Create Phase Models
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			m_PhaseModels[phase_types[i]] = new CubicEoSPhaseModel(comp_properties, eos_types[i], phase_types[i]);
		}

		//Create Phases Properties
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			m_PhasesProperties[phase_types[i]] = new PhaseProperties();
		}

		//Create Flash pointer
		switch (flash_type)
		{
		case COMPOSITIONAL_FLASH_TYPE::TRIVIAL:
			{
				m_CompositionalFlash = new TrivialFlash(m_PhaseModels);
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

	void CompositionalMultiphaseSystem::Update(double pressure, double temperature, std::vector<double> feed)
	{
		m_CompositionalFlash->ComputeEquilibrium(pressure, temperature, feed, &m_MultiphaseProperties);
		for (auto it = m_PhaseModels.begin(); it != m_PhaseModels.end(); ++it)
		{
			it->second->ComputeAllProperties(pressure, temperature, feed, m_PhasesProperties[it->first]);
		}

	}

	/*void CompositionalMultiphaseSystem::Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables)
	{
		m_CompositionalFlash->ComputeEquilibrium(pressure,temperature,feed, out_variables);
	}*/

}


