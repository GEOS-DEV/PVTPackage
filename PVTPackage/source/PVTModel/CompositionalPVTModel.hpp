#pragma once
#include "PVTModel/PVTModel.hpp"
#include "PVTModel/ComponentProperties.hpp"
#include "PVTModel/Flash/CompositionalFlash.hpp"

namespace PVTPackage
{
	class CompositionalPVTModel final: public PVTModel
	{
	public:
		CompositionalPVTModel(std::vector<PHASE_TYPE> phase_types,
		                       std::vector<EOS_TYPE> eos_types, COMPOSITIONAL_FLASH_TYPE flash_type,
		                       ComponentProperties comp_properties);

		~CompositionalPVTModel();

		void Flash(const FlashInputVariables& in_variables, FlashOutputVariables& out_variables) override;

	protected:

		ComponentProperties m_ComponentProperties;
		COMPOSITIONAL_FLASH_TYPE m_FlashType;
		std::unordered_map<PHASE_TYPE, EOS_TYPE> m_EoSTypes;
		CompositionalFlash* m_CompositionalFlash;

	};

}
