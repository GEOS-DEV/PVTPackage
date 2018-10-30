#pragma once
#include <unordered_map>
#include <list>
#include "MultiphaseSystem/PVTEnums.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"


namespace PVTPackage
{
	struct MultiphaseSystemProperties;

	class CompositionalFlash
	{
	public:

		virtual ~CompositionalFlash() = default;

		CompositionalFlash(const ComponentProperties& component_properties);

		bool ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties & out_variables);
		double SolveRachfordRiceEquation(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index);
		double RachfordRiceFunction(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, double x);
		double dRachfordRiceFunction_dx(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, double x);

		virtual void set_PhaseState(MultiphaseSystemProperties& out_variables) = 0;
		virtual bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) = 0;

	protected:

		ComponentProperties m_ComponentsProperties;

		//Wilson K-values
		std::vector<double> ComputeWilsonGasOilKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterGasKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterOilKvalue(double Pressure, double Temperature) const;

		bool ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties & out_variables);
	};
}
