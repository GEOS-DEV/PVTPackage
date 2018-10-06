#pragma once
#include "Utils/Assert.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <vector>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <map>


namespace PVTPackage
{
	struct PhaseProperties;

	class BlackOil_GasModel final : public PhaseModel
	{
	public:

		BlackOil_GasModel(std::vector<std::vector<double>> PVTG, double surface_density, double mw);

		~BlackOil_GasModel() override = default;

		void ComputeAllProperties(double Pressure, double Temperature, std::vector<double>& composition, PhaseProperties& props_out) override {}

	protected:

		//Table stored as a map, Pb is the key
		std::vector<double> m_Pdew;
		std::vector<std::vector<std::vector<double>>> m_PVTGTable;
		void CheckTableConsistency();
		

	};

	
}
