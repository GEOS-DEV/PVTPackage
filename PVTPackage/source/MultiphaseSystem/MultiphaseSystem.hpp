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

		MultiphaseSystem(size_t nc, std::vector<PHASE_TYPE> phase_types) :
			 m_MultiphaseProperties(phase_types,nc)

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
			return m_MultiphaseProperties.PhasesProperties.at(phase_type);
		}

	protected:

		//Properties
		MultiphaseSystemProperties m_MultiphaseProperties;

	};

}
