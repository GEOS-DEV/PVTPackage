#pragma once
#include "Utils/Assert.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <map>


namespace PVTPackage
{

	class BlackOil_WaterModel final : public PhaseModel
	{
	public:

		BlackOil_WaterModel(std::vector<double> PVTW, double surface_density, double mw);

		~BlackOil_WaterModel() override = default;

		void ComputeAllProperties(double Pressure, double Temperature, std::vector<double>& composition, PhaseProperties& props_out) override {}

	protected:

		std::map<double, std::vector<std::vector<double>>> m_PVTO;

	};

	
}
