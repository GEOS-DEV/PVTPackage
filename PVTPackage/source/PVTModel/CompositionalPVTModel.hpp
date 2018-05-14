#pragma once
#include "PVTModel/PVTModel.hpp"
#include "PVTModel/ComponentProperties.hpp"
#include "PVTModel/PhaseSplitModel/CompositionalFlash.hpp"
#include "PVTModel/PhaseModel/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{
	class CompositionalPVTModel final: public PVTModel
	{
	public:

		CompositionalPVTModel(std::vector<PHASE_TYPE> phase_types,
		                       std::vector<EOS_TYPE> eos_types, COMPOSITIONAL_FLASH_TYPE flash_type,
		                       ComponentProperties comp_properties);

		~CompositionalPVTModel();

		void Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables) override;

	protected:

		//Components properties (Tc,Pc...)
		ComponentProperties m_ComponentProperties;
		
		//Eos types per phase
		std::unordered_map<PHASE_TYPE, EOS_TYPE> m_EoSTypes;
		std::unordered_map<PHASE_TYPE, CubicEoSPhaseModel*> m_PhaseModel;

		//Flash type and pointer
		COMPOSITIONAL_FLASH_TYPE m_FlashType;
		CompositionalFlash* m_CompositionalFlash;

		
	};

}
