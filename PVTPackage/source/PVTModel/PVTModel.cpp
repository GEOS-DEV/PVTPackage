#include "PVTModel/PVTModel.hpp"

namespace PVTPackage
{

	PVTModel::~PVTModel()
	{
	}

	PVTModel::PVTModel(size_t n_components, std::vector<PHASE_TYPE> phase_types):
		m_NComponents(n_components),
		m_NMaxPhases(phase_types.size()),
		m_PhaseTypes(phase_types)
	{
		
	}

}
