#include "MultiphaseSystem/CompositionalMultiphaseSystem.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{
	CompositionalMultiphaseSystem::CompositionalMultiphaseSystem( std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
	                                               COMPOSITIONAL_FLASH_TYPE flash_type,
	                                               const ComponentProperties& comp_properties):
		MultiphaseSystem(comp_properties.NComponents, phase_types),
		m_CompositionalFlash(nullptr)
	{
		
		//Create Phase Models
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			m_MultiphaseProperties.PhaseModels[phase_types[i]] = new CubicEoSPhaseModel(comp_properties, eos_types[i], phase_types[i]);
		}

		//Create Phases Properties
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			m_MultiphaseProperties.PhasesProperties[phase_types[i]] = new PhaseProperties(comp_properties.NComponents);
		}

		//Create Flash pointer
		switch (flash_type)
		{
		case COMPOSITIONAL_FLASH_TYPE::TRIVIAL:
			{
				m_CompositionalFlash = new TrivialFlash(comp_properties);
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
		m_MultiphaseProperties.Temperature = temperature;
		m_MultiphaseProperties.Pressure = pressure;
		m_MultiphaseProperties.Feed = feed;

		//Multiphase Properties
		m_CompositionalFlash->ComputeEquilibriumAndDerivatives(m_MultiphaseProperties);
	}



}


