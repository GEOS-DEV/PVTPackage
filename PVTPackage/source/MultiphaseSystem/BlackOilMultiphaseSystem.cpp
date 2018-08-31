#include "MultiphaseSystem/BlackOilMultiphaseSystem.hpp"
#include <unordered_map>
#include "PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_WaterModel.hpp"

namespace PVTPackage
{

	BlackOilMultiphaseSystem::BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
		std::vector<std::vector<double>> PVTO, std::vector<double> PVTW, std::vector<std::vector<double>> PVTG,
		std::vector<double> DENSITY, std::vector<double> MW) : MultiphaseSystem(phase_types.size(), phase_types.size()),
		                                                       m_BlackOilFlash(nullptr)
	{
		//Phase to index mapping
		std::unordered_map<PHASE_TYPE, int> phase_to_index;
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			phase_to_index[phase_types[i]] = i;
		}


		//Create Phase Models
		for (size_t i = 0; i != phase_types.size(); ++i)
		{
			if (phase_types[i] == PHASE_TYPE::OIL)
			{
				m_PhaseModels[PHASE_TYPE::OIL] = new BlackOil_OilModel(PVTO, DENSITY[i], MW[i], DENSITY[phase_to_index[PHASE_TYPE::GAS]], MW[phase_to_index[PHASE_TYPE::GAS]]);
			}
			else if (phase_types[i] == PHASE_TYPE::GAS)
			{
				m_PhaseModels[PHASE_TYPE::GAS] = new BlackOil_GasModel(PVTG, DENSITY[i], MW[i]);
			}
			else if (phase_types[i] == PHASE_TYPE::LIQUID_WATER_RICH)
			{
				m_PhaseModels[PHASE_TYPE::LIQUID_WATER_RICH] = new BlackOil_WaterModel(PVTW, DENSITY[i], MW[i]);
			}
			else
			{
				LOGERROR("Phase type not supported for Black Oil model");
			}
		}
		
		//Check consistency between PVTO and PVTG
		//TODO


		//Check if both oil and gas are defined
		ASSERT((m_PhaseModels.find(PHASE_TYPE::OIL) != m_PhaseModels.end()) && (m_PhaseModels.find(PHASE_TYPE::GAS) != m_PhaseModels.end()), "Both oil and gas phase must be defined for BO model");

		//Create Flash pointer

	}

	


	//void CompositionalMultiphaseSystem::Update(double pressure, double temperature, std::vector<double> feed)
	//{
	//	m_CompositionalFlash->ComputeEquilibrium(pressure, temperature, feed, &m_MultiphaseProperties);
	//	for (auto it = m_PhaseModels.begin(); it != m_PhaseModels.end(); ++it)
	//	{
	//		it->second->ComputeAllProperties(pressure, temperature, feed, m_PhasesProperties[it->first]);
	//	}

	//}

	///*void CompositionalMultiphaseSystem::Flash(double pressure, double temperature, std::vector<double> feed, PhaseSplitModelOutputVariables& out_variables)
	//{
	//	m_CompositionalFlash->ComputeEquilibrium(pressure,temperature,feed, out_variables);
	//}*/


}


