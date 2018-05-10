#include "PVTModel/CompositionalPVTModel.hpp"
#include "PVTModel/Flash/CompositionalFlash.hpp"
#include "PVTModel/Flash/TrivialFlash.hpp"

namespace PVTPackage
{
	CompositionalPVTModel::CompositionalPVTModel( std::vector<PHASE_TYPE> phase_types, std::vector<EOS_TYPE> eos_types,
	                                               COMPOSITIONAL_FLASH_TYPE flash_type,
	                                               ComponentProperties comp_properties):
		PVTModel(comp_properties.NComponents, phase_types), m_ComponentProperties(std::move(comp_properties)),
		m_FlashType(flash_type),
		m_CompositionalFlash(nullptr)
	{
		//Assign phase type to eos models
		ASSERT(phase_types.size() == eos_types.size(), "EOS types and Phase type must be same dimension.");
		for (size_t i = 0; i != eos_types.size(); ++i)
		{
			m_EoSTypes[phase_types[i]] = eos_types[i];
		}


		//Create Flash pointer
		switch (m_FlashType)
		{
		case COMPOSITIONAL_FLASH_TYPE::TRIVIAL:
			{
				m_CompositionalFlash = new TrivialFlash(phase_types, eos_types, &m_ComponentProperties);
				break;
			}
		case COMPOSITIONAL_FLASH_TYPE::FREE_WATER_FLASH:
			break;
		default:
			LOGERROR("Flash type is not correct or not supported.");
		}
	}

	CompositionalPVTModel::~CompositionalPVTModel()
	{
		delete m_CompositionalFlash;
	}

	void CompositionalPVTModel::Flash(const FlashInputVariables& in_variables, FlashOutputVariables& out_variables)
	{
		m_CompositionalFlash->ComputeEquilibrium(in_variables, out_variables);
	}

}


