#include "MultiphaseSystem/MultiphaseSystem.hpp"

namespace PVTPackage
{
	
	void MultiphaseSystem::Update(double pressure, double temperature, std::vector<double> feed)
	{
	  ASSERT(m_Flash != nullptr, "Flash has not been initialized");

		m_MultiphaseProperties.Temperature = temperature;
		m_MultiphaseProperties.Pressure = pressure;
		m_MultiphaseProperties.Feed = feed;

		//Multiphase Properties
		const bool res = m_Flash->ComputeEquilibriumAndDerivatives(m_MultiphaseProperties);

		m_StateIndicator = res ? State::SUCCESS : State::NOT_CONVERGED;
	}

}
