#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include  "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "Utils/math.hpp"

namespace PVTPackage
{

	CompositionalFlash::CompositionalFlash(const ComponentProperties& component_properties): m_ComponentsProperties(component_properties)
	{
		////Check components properties are the same
		//for (auto it = phase_models.begin(); it != std::prev(phase_models.end()); ++it)
		//{
		//	auto phase1 = dynamic_cast<CubicEoSPhaseModel*>(it->second);
		//	auto phase2 = dynamic_cast<CubicEoSPhaseModel*>(std::next(it)->second);
		//	ASSERT(phase1->get_ComponentsProperties() == phase2->get_ComponentsProperties(),"Different component properties in flash");

		//}

		////If so define attribute to be used as global properties for flash calculations
		//m_ComponentsProperties = &dynamic_cast<CubicEoSPhaseModel*>(phase_models.begin()->second)->get_ComponentsProperties();
	}

	void CompositionalFlash::ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables)
	{
		//Compute Equilibrium
		ComputeEquilibrium(out_variables);

		//Finite difference derivatives
		ComputeFiniteDifferenceDerivatives(out_variables);

	}

	std::vector<double> CompositionalFlash::ComputeWilsonGasOilKvalue(double Pressure, double Temperature) const
	{
		const auto nbc = m_ComponentsProperties.NComponents;
		const auto& Tc = m_ComponentsProperties.Tc;
		const auto& Pc = m_ComponentsProperties.Pc;
		const auto& Omega = m_ComponentsProperties.Omega;

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
		const auto nbc = m_ComponentsProperties.NComponents;
		const auto water_index = m_ComponentsProperties.WaterIndex;
		std::vector<double> Kval(nbc, 0);
		Kval[water_index] = exp(-4844.168051 / Temperature + 12.93022442)*1e5 / Pressure;
		return Kval;

	}

	std::vector<double> CompositionalFlash::ComputeWaterOilKvalue(double Pressure, double Temperature) const
	{
		const auto nbc = m_ComponentsProperties.NComponents;
		return std::vector<double>(nbc, 0);
	}

	void CompositionalFlash::ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables)
	{
		const auto& pressure = out_variables.Pressure;
		const auto& temperature = out_variables.Temperature;
		const auto& feed = out_variables.Feed;

		auto  sqrtprecision = sqrt(std::numeric_limits<double>::epsilon());
		double epsilon = 0;

		MultiphaseSystemProperties props_eps = out_variables;

		////Pressure
		epsilon = sqrtprecision * (abs(pressure) + sqrtprecision);
		props_eps.Pressure = pressure + epsilon;
		ComputeEquilibrium(props_eps);
		props_eps.Pressure = pressure;
		out_variables.UpdateDerivative_dP_FiniteDifference(props_eps, epsilon);

		////Temperature
		epsilon = sqrtprecision * (abs(temperature) + sqrtprecision);
		props_eps.Temperature = temperature + epsilon;
		ComputeEquilibrium(props_eps);
		props_eps.Temperature = temperature;
		out_variables.UpdateDerivative_dT_FiniteDifference(props_eps, epsilon);

		//Feed
		for (size_t i=0; i<feed.size();++i)
		{
			epsilon = sqrtprecision * (abs(feed[i]) + sqrtprecision);
			if (feed[i] + epsilon > 1)
			{
				epsilon = -epsilon;
			}
			auto save_feed = feed;
			props_eps.Feed[i] = feed[i] + epsilon;
			props_eps.Feed = math::Normalize(props_eps.Feed);
			ComputeEquilibrium(props_eps);
			props_eps.Feed = save_feed;
			out_variables.UpdateDerivative_dz_FiniteDifference(i,props_eps, epsilon);
		}

		auto gg = 2;

	}


}


