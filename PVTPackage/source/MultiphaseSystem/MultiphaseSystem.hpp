#pragma once
#include <vector>
#include "MultiphaseSystem/PVTEnums.hpp"
#include "MultiphaseSystemProperties.hpp"
#include "PhaseModel/PhaseProperties.hpp"

namespace PVTPackage
{


	class PhaseModel;

	class MultiphaseSystem
	{
	public:
		virtual ~MultiphaseSystem() = default;

		MultiphaseSystem(size_t nc, size_t np) :
			m_NComponents(nc),
			m_NMaxPhases(np)
		{
		}

		virtual void Update(double pressure, double temperature, std::vector<double> feed)=0;

		//--Getters
		const MultiphaseSystemProperties* get_MultiphaseSystemProperties() const
		{
			return &m_MultiphaseProperties;
		}

		const PhaseProperties* get_PhaseProperties(PHASE_TYPE phase_type)
		{
			return m_PhasesProperties.at(phase_type);
		}

	protected:

		//Dimensions
		size_t m_NComponents;
		size_t m_NMaxPhases;

		//PhaseModels
		std::unordered_map<PHASE_TYPE, PhaseModel*> m_PhaseModels;

		//Properties -- OUTPUT
		//--Multiphase (Phase fractions, compositions...)
		MultiphaseSystemProperties m_MultiphaseProperties;

		//--Phase
		std::unordered_map<PHASE_TYPE, PhaseProperties*> m_PhasesProperties;

	};

}
