#pragma once
#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "PhaseSplitModel/BlackOilFlash.hpp"

namespace PVTPackage
{
	class BlackOilMultiphaseSystem final : public MultiphaseSystem
	{
	public:

		BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
			std::vector<std::vector<double>> PVTO,
			std::vector<double> PVTW,
			std::vector<std::vector<double>> PVTG,
			std::vector<double> DENSITY,
			std::vector<double> MW);

		BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
			std::vector<std::string> table_file_names,
			std::vector<double> DENSITY,
			std::vector<double> MW);

		~BlackOilMultiphaseSystem() override;


	protected:

		void CreatePhases(std::vector<PHASE_TYPE> phase_types,
			std::vector<std::vector<double>> PVTO,
			std::vector<double> PVTW,
			std::vector<std::vector<double>> PVTG,
			std::vector<double> DENSITY,
			std::vector<double> MW);


	};


}