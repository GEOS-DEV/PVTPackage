#include "BlackOilMultiphaseSystem.hpp"

#include <unordered_map>
#include "PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_WaterModel.hpp"
#include "Utils/math.hpp"

namespace PVTPackage
{

	BlackOilMultiphaseSystem::BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
		std::vector<std::vector<double>> PVTO, std::vector<double> PVTW, std::vector<std::vector<double>> PVTG,
		std::vector<double> DENSITY, std::vector<double> MW) : MultiphaseSystem(phase_types.size(), phase_types)

	{

		//Create Phase Models
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			if (phase_types[i] == PHASE_TYPE::OIL)
			{
				m_MultiphaseProperties.PhaseModels[PHASE_TYPE::OIL] = new BlackOil_OilModel(PVTO, DENSITY[i], MW[i]);
			}
			else if (phase_types[i] == PHASE_TYPE::GAS)
			{
				m_MultiphaseProperties.PhaseModels[PHASE_TYPE::GAS] = new BlackOil_GasModel(PVTG, DENSITY[i], MW[i]);
			}
			else if (phase_types[i] == PHASE_TYPE::LIQUID_WATER_RICH)
			{
				m_MultiphaseProperties.PhaseModels[PHASE_TYPE::LIQUID_WATER_RICH] = new BlackOil_WaterModel(PVTW, DENSITY[i], MW[i]);
			}
			else
			{
				LOGERROR("Phase type not supported for Black Oil model");
			}

		}
	}

	BlackOilMultiphaseSystem::~BlackOilMultiphaseSystem()
	{
		delete m_Flash;
	}


}


