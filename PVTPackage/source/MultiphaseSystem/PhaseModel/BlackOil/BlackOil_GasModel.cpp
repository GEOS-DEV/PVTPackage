#include <algorithm>
#include "Utils/math.hpp"
#include "BlackOil_Utils.hpp"
#include "BlackOil_GasModel.hpp"

namespace PVTPackage
{
	
	BlackOil_GasModel::BlackOil_GasModel(std::vector<std::vector<double>> PVTO, double surface_density, double mw)
	{

		////--Fill table
		//BlackOilUtils::CreateTable(PVTO, m_PVTGTable, m_Pdew);

		////Check Consistency
		//CheckTableConsistency();

		////Create missing undersaturated branches
		//BlackOilUtils::CreateUnderSaturatedProperties(m_PVTGTable, m_Pdew);
		//
		////Refine Table branches
		//BlackOilUtils::RefineTable(m_PVTGTable,100);

		////Check Consistency
		//CheckTableConsistency();

		/////DEBUG PURPOSE - PLOT table for matlab
		//std::ofstream outputFile("PVTG.txt");
		//for (size_t i =0 ; i<m_PVTGTable.size(); ++i )
		//{
		//	for (size_t j = 0; j < m_PVTGTable[i][0].size(); ++j)
		//	{
		//		outputFile << m_Pdew[i] << " " << m_PVTGTable[i][0][j] << " " << m_PVTGTable[i][1][j] << " " << m_PVTGTable[i][2][j] << "\n";
		//	}
		//}

	}

	void BlackOil_GasModel::CheckTableConsistency()
	{
		////--Check for the presence of one bubble point
		//ASSERT(m_PVTOTable[m_PVTOTable.size()-1][0].size() > 1, "At least one bubble is required");


		//for (size_t i = 0; i < m_PVTGTable.size()-1; ++i)
		//{
		//	auto Rs = m_Rs;
		//	auto Pb = m_PVTOTable[i][0][0];
		//	auto Bo = m_PVTOTable[i][1];
		//	auto Visc = m_PVTOTable[i][2];

		//	//--Saturated
		//	//Rs must increase with Pb
		//	ASSERT((m_Rs[i+1] - m_Rs[i])>0, "Rs must increase with Pb");
		//	//Bo must increase with Pb
		//	ASSERT((m_PVTOTable[i+1][1][0] - m_PVTOTable[i][1][0])>0, "Bo must increase with Pb in saturated region");
		//	//Visc must decrease with Pb
		//	ASSERT((m_PVTOTable[i+1][2][0] - m_PVTOTable[i][2][0])<0, "Viscosity must increase with Pb in saturated region");
		//	//--Undersaturated
		//	for (size_t j = 0; j < m_PVTOTable[i][0].size() - 1; ++j)
		//	{
		//		//Bo must decrease with P
		//		ASSERT((Bo[j + 1] - Bo[j])<0, "Bo must decrease with P in undersaturated region");
		//		//Visc must increase with Pb
		//		ASSERT((Visc[j + 1] - Visc[j])>0, "Viscosity must increase with P in undersaturated region");
		//	}
		//}

	}
}
