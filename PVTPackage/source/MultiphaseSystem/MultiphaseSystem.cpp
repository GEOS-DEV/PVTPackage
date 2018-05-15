#include "MultiphaseSystem/MultiphaseSystem.hpp"

namespace PVTPackage
{

	MultiphaseSystem::~MultiphaseSystem()
	{
	}

	MultiphaseSystem::MultiphaseSystem(size_t n_components, std::vector<PHASE_TYPE> phase_types):
		m_NComponents(n_components),
		m_NMaxPhases(phase_types.size()),
		m_Phases(phase_types)
	{
		
	}

}
