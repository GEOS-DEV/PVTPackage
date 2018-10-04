#pragma once
#include <unordered_map>
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

		void ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);

		virtual void ComputeEquilibrium(MultiphaseSystemProperties& out_variables)=0;

	protected:

		const ComponentProperties& m_ComponentsProperties;

		//Wilson K-values
		std::vector<double> ComputeWilsonGasOilKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterGasKvalue(double Pressure, double Temperature) const;
		std::vector<double> ComputeWaterOilKvalue(double Pressure, double Temperature) const;

		void ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables);
	};
}
