#pragma once
#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "PhaseSplitModel/BlackOilFlash.hpp"

namespace PVTPackage
{
	class BlackOilMultiphaseSystem final: public MultiphaseSystem
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

		//void Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables) override;

	protected:

<<<<<<< HEAD
=======
		void CreatePhases(std::vector<PHASE_TYPE> phase_types,
											std::vector<std::vector<double>> PVTO,
											std::vector<double> PVTW,
											std::vector<std::vector<double>> PVTG,
											std::vector<double> DENSITY,
											std::vector<double> MW);

		//Flash pointer
		//BlackOilFlash* m_BlackOilFlash;

>>>>>>> 489cf64f5560b9a2d2f0f4030fb7292341915c23
		
	};


}
