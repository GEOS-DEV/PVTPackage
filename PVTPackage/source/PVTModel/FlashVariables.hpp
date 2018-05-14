#pragma once
#include <unordered_map>
#include "PVTModel/PVTEnums.hpp"

namespace PVTPackage
{
	class FlashInputVariables
	{
	public:

		FlashInputVariables(size_t nc)
		{
			m_NComponents = nc;
			m_Pressure = 0;
			m_Temperature = 0;
			m_Feed = std::vector<double>(nc);

		}

		//--Setters
		//Set Temperature, Pressure and feed
		void SetPressureTemperatureFeed(double pressure, double temperature, std::vector<double> feed)
		{
			m_Temperature = temperature;
			m_Pressure = pressure;
			m_Feed = feed;
		}

		//Set Pressure and feed
		void SetPressureTemperatureFeed(double pressure, std::vector<double> feed)
		{
			m_Pressure = pressure;
			m_Feed = feed;
		}

		//Set Temperature
		void SetTemperature(double temperature)
		{
			m_Temperature = temperature;
		}

		//Set Pressure
		void SetPressure(double pressure)
		{
			m_Pressure = pressure;
		}

		//Set Feed
		void SetFeed(std::vector<double> feed)
		{
			m_Feed = feed;
		}

		//--Getters
		const double& get_Pressure() const
		{
			return m_Pressure;
		}

		const double& get_Temperature() const
		{
			return m_Temperature;
		}

		const std::vector<double>& get_Feed() const
		{
			return m_Feed;
		}



	protected:
		size_t m_NComponents;
		double m_Pressure;
		double m_Temperature;
		std::vector<double> m_Feed;

	};


	class FlashOutputVariables
	{
	public:
		FlashOutputVariables(FlashInputVariables* input_variables): m_PhaseState(PHASE_STATE::UNKNOWN), m_InputVariables(input_variables)
		{
		}

		//Getters
		//--Vector of mole fraction of 'phase' 
		 double& get_MoleFraction(PHASE_TYPE phase)
		{
			return m_MoleFraction[phase];
		}

		//--Vector of Phase composition (mol/mol) of 'phase' 
		 std::vector<double>& get_Composition(PHASE_TYPE phase)
		{
			return m_MoleComposition[phase];
		}

		 //--Vector of Phase composition (mol/mol) of 'phase' 
		 void set_PhaseState()
		 {
			 m_PhaseState = PhaseStateMap.at({ m_MoleFraction[PHASE_TYPE::OIL] > 0., m_MoleFraction[PHASE_TYPE::GAS] > 0., m_MoleFraction[PHASE_TYPE::LIQUID_WATER_RICH] > 0.});
		 }

	protected:
		
		//Primary
		PHASE_STATE m_PhaseState;
		std::unordered_map<PHASE_TYPE, std::vector<double>> m_MoleComposition;
		std::unordered_map<PHASE_TYPE, double> m_MoleFraction;
		const FlashInputVariables* m_InputVariables;

	};

}



