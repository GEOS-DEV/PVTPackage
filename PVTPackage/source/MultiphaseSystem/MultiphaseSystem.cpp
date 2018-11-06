#include "MultiphaseSystem/MultiphaseSystem.hpp"

namespace PVTPackage
{
	
	void MultiphaseSystem::Update(double pressure, double temperature, std::vector<double> feed)
	{
		m_MultiphaseProperties.Temperature = temperature;
		m_MultiphaseProperties.Pressure = pressure;
		m_MultiphaseProperties.Feed = feed;

		//Multiphase Properties
		m_Flash->ComputeEquilibriumAndDerivatives(m_MultiphaseProperties);
	}

}
