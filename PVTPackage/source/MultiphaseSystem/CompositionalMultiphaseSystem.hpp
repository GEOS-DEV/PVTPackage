#pragma once
#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

namespace PVTPackage
{
	class CompositionalMultiphaseSystem final: public MultiphaseSystem
	{
	public:

		CompositionalMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
		                       std::vector<EOS_TYPE> eos_types, COMPOSITIONAL_FLASH_TYPE flash_type,
		                       ComponentProperties comp_properties);

		~CompositionalMultiphaseSystem();

		void Update(double pressure, double temperature, std::vector<double> feed) override;

		//void Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables) override;

	protected:

		//Flash pointer
		CompositionalFlash* m_CompositionalFlash;

		
	};

}
