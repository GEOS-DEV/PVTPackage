#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"

namespace PVTPackage
{
	CompositionalFlash::~CompositionalFlash()
	{
		
	}

	CompositionalFlash::CompositionalFlash(std::unordered_map<PHASE_TYPE, PhaseModel*>& phase_models)
	{
		//Check components properties are the same
		for (auto it = phase_models.begin(); it != std::prev(phase_models.end()); ++it)
		{
			auto phase1 = dynamic_cast<CubicEoSPhaseModel*>(it->second);
			auto phase2 = dynamic_cast<CubicEoSPhaseModel*>(std::next(it)->second);
			ASSERT(phase1->get_ComponentsProperties() == phase2->get_ComponentsProperties(),"Different component properties in flash");
			
		}

		//If so define attribute to be used as global properties for flash calculations
		m_ComponentsProperties = &dynamic_cast<CubicEoSPhaseModel*>(phase_models.begin()->second)->get_ComponentsProperties();

		for (auto& phase_model : phase_models)
		{
			m_PhaseModels[phase_model.first] = static_cast<PhaseModel*>(phase_model.second);
		}

	}

	std::vector<double> CompositionalFlash::ComputeWilsonGasOilKvalue(double Pressure, double Temperature) const
	{

		const auto nbc = m_ComponentsProperties->NComponents;
		const auto& Tc = m_ComponentsProperties->Tc;
		const auto& Pc = m_ComponentsProperties->Pc;
		const auto& Omega = m_ComponentsProperties->Omega;

		std::vector<double> Kval(nbc);

		//Gas-Oil 
		for (size_t i = 0; i != nbc; i++)
		{
			Kval[i] = Pc[i] / Pressure * exp(5.37*(1 + Omega[i])*(1 - Tc[i] / Temperature));
		}

		return Kval;
	}

	std::vector<double> CompositionalFlash::ComputeWaterGasKvalue(double Pressure, double Temperature) const
	{
		const auto nbc = m_ComponentsProperties->NComponents;
		const auto water_index = m_ComponentsProperties->WaterIndex;
		std::vector<double> Kval(nbc, 0);
		Kval[water_index] = exp(-4844.168051 / Temperature + 12.93022442)*1e5 / Pressure;
		return Kval;

	}

	std::vector<double> CompositionalFlash::ComputeWaterOilKvalue(double Pressure, double Temperature) const
	{
		const auto nbc = m_ComponentsProperties->NComponents;
		return std::vector<double>(nbc, 0);
	}

}


