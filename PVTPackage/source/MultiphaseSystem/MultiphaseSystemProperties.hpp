#pragma once
#include <unordered_map>
#include "MultiphaseSystem/PVTEnums.hpp"
#include "PhaseSplitModel/CompositionalFlash.hpp"
#include <utility>
#include "PhaseModel/PhaseProperties.hpp"

namespace PVTPackage
{
	class PhaseModel;

	struct MultiphaseSystemProperties
	{
		MultiphaseSystemProperties(std::vector<PHASE_TYPE> phase_types, size_t ncomponents) :	Temperature(0),
																								Pressure(0),
																								Feed(std::vector<double>(ncomponents,0)),
																								PhaseState(PHASE_STATE::UNKNOWN),
																								PhaseTypes(phase_types)
		{
			for (auto& it : PhaseTypes)
			{
				PhaseMoleFraction.insert(std::pair<PHASE_TYPE, ScalarPropertyAndDerivatives<double>>(it, ScalarPropertyAndDerivatives<double>(ncomponents)));
			}

			//Create Phases Properties
			for (size_t i = 0; i != phase_types.size(); ++i)
			{
				PhasesProperties.insert(std::pair<PHASE_TYPE, PhaseProperties>(phase_types[i], PhaseProperties(ncomponents)));
			}
		}

		void UpdateDerivative_dP_FiniteDifference(const MultiphaseSystemProperties& props_eps, double epsilon)
		{
			for (auto phase : PhaseTypes)
			{
				PhaseMoleFraction.at(phase).dP = (props_eps.PhaseMoleFraction.at(phase).value - PhaseMoleFraction.at(phase).value) / epsilon;
				PhasesProperties.at(phase).UpdateDerivative_dP_FiniteDifference(props_eps.PhasesProperties.at(phase), epsilon);
			}
		}

		void UpdateDerivative_dT_FiniteDifference(const MultiphaseSystemProperties& props_eps, double epsilon)
		{
			for (auto phase : PhaseTypes)
			{
				PhaseMoleFraction.at(phase).dT = (props_eps.PhaseMoleFraction.at(phase).value - PhaseMoleFraction.at(phase).value) / epsilon;
				PhasesProperties.at(phase).UpdateDerivative_dT_FiniteDifference(props_eps.PhasesProperties.at(phase), epsilon);
			}
		}

		void UpdateDerivative_dz_FiniteDifference(size_t i, const MultiphaseSystemProperties& props_eps, double epsilon)
		{
			for (auto phase : PhaseTypes)
			{
				PhaseMoleFraction.at(phase).dz[i] = (props_eps.PhaseMoleFraction.at(phase).value - PhaseMoleFraction.at(phase).value) / epsilon;
				PhasesProperties.at(phase).UpdateDerivative_dz_FiniteDifference(i,props_eps.PhasesProperties.at(phase), epsilon);
			}
		}


		//State Variables
		double Temperature;
		double Pressure;
		std::vector<double> Feed;

		//Phase state
		PHASE_STATE PhaseState;

		//Phase types
		std::vector<PHASE_TYPE> PhaseTypes;

		//PhaseModels
		std::unordered_map<PHASE_TYPE, PhaseModel*> PhaseModels;

		//System properties
		std::unordered_map <PHASE_TYPE, ScalarPropertyAndDerivatives<double>> PhaseMoleFraction;
		
		//Phases properties
		std::unordered_map<PHASE_TYPE, PhaseProperties> PhasesProperties;

	};




}



