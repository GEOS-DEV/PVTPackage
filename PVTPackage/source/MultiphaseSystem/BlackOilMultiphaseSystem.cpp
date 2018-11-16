#include "BlackOilMultiphaseSystem.hpp"

#include <unordered_map>
#include "PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_WaterModel.hpp"

namespace PVTPackage
{

	BlackOilMultiphaseSystem::BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
		std::vector<std::vector<double>> PVTO, std::vector<std::vector<double>> PVTG, std::vector<double> PVTW,
		std::vector<double> DENSITY, std::vector<double> MW)
		: MultiphaseSystem(phase_types.size(), phase_types)
	{
		CreatePhases(phase_types, PVTO, PVTW, PVTG, DENSITY, MW);

		//Create Flash pointer
		m_Flash = new BlackOilFlash();
	}

	BlackOilMultiphaseSystem::BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
		std::vector<std::string> table_file_names, std::vector<double> DENSITY, std::vector<double> MW)
		: MultiphaseSystem(phase_types.size(), phase_types)
	{
		// TODO read tables
		  //CreatePhases(phase_types, PVTO, PVTG, PVTW, DENSITY, MW);

		  //Create Flash pointer
		m_Flash = new BlackOilFlash();
	}

	void BlackOilMultiphaseSystem::CreatePhases(std::vector<PHASE_TYPE> phase_types,
		std::vector<std::vector<double>> PVTO,
		std::vector<double> PVTW,
		std::vector<std::vector<double>> PVTG,
		std::vector<double> DENSITY,
		std::vector<double> MW)
	{
		//Phase to index mapping
		std::unordered_map<PHASE_TYPE, int> phase_to_index;
		for (int i = 0; i < static_cast<int>(phase_types.size()); ++i)
		{
			phase_to_index[phase_types[i]] = static_cast<int>(i);
		}


		//Create Phase Models
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			if (phase_types[i] == PHASE_TYPE::OIL)
			{
				m_MultiphaseProperties.PhaseModels[PHASE_TYPE::OIL] = new BlackOil_OilModel(PVTO,
					DENSITY[i],
					MW[i]);
			}
			else if (phase_types[i] == PHASE_TYPE::GAS)
			{
				m_MultiphaseProperties.PhaseModels[PHASE_TYPE::GAS] = new BlackOil_GasModel(PVTG,
					DENSITY[i],
					MW[i]);
			}
			else if (phase_types[i] == PHASE_TYPE::LIQUID_WATER_RICH)
			{
				m_MultiphaseProperties.PhaseModels[PHASE_TYPE::LIQUID_WATER_RICH] = new BlackOil_WaterModel(PVTW,
					DENSITY[i],
					MW[i]);
			}
			else
			{
				LOGERROR("Phase type not supported for Black Oil model");
			}
		}

		//Check consistency between PVTO and PVTG
		//TODO


		//Check if both oil and gas are defined
		ASSERT((m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::OIL) != m_MultiphaseProperties.PhaseModels.end())
			&& (m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::GAS) != m_MultiphaseProperties.PhaseModels.end()),
			"Both oil and gas phase must be defined for BO model");

	}


	BlackOilMultiphaseSystem::~BlackOilMultiphaseSystem()
	{
		delete m_Flash;
	}


}