#include <algorithm>
#include "Utils/math.hpp"
#include "BlackOil_Utils.hpp"
#include "BlackOil_GasModel.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseProperties.hpp"

namespace PVTPackage
{
	
	BlackOil_GasModel::BlackOil_GasModel(std::vector<std::vector<double>> PVTG, double gas_surface_mass_density, double gas_surface_mw)
		: min_Pressure(-1), max_Pressure(-1), m_SurfaceMassDensity(0), m_SurfaceMoleDensity(0), m_SurfaceMolecularWeight(gas_surface_mw)
	{

		//--Fill table
		CreateTable(PVTG);

		//Check Consistency
		//CheckTableConsistency();

		////Create missing undersaturated branches
		//CreateUnderSaturatedProperties();

		////Extend existing branches
		//ExtendUnderSaturatedProperties();

		////Refine Table branches
		//RefineTable(100);

		//Check Consistency
		//CheckTableConsistency();


		///DEBUG PURPOSE - PLOT table for matlab
//		std::ofstream outputFile("PVTG.txt");
//		for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
//		{
//			for (size_t j = 0; j < m_PVTG.UndersaturatedRv[i].size(); ++j)
//			{
//				outputFile << m_PVTG.DewPressure[i] << " " << m_PVTG.UndersaturatedRv[i][j] + m_PVTG.Rv[i] << " " << m_PVTG.UndersaturatedBg[i][j] << " " << m_PVTG.UndersaturatedViscosity[i][j] << "\n";
//			}
//		}


		//Density
		m_SurfaceMassDensity = gas_surface_mass_density;
		m_SurfaceMoleDensity = m_SurfaceMassDensity / m_SurfaceMolecularWeight;

	}

	void BlackOil_GasModel::CheckTableConsistency()
	{
		//--Check for the presence of one bubble point
		ASSERT(m_PVTG.UndersaturatedRv[m_PVTG.NSaturatedPoints - 1].size() > 1, "At least one bubble pressure is required");


		//Check for saturated region
		for (size_t i = 0; i < m_PVTG.NSaturatedPoints - 1; ++i)
		{
			//Rv must increase with Pdew
			//ASSERT((m_PVTG.Rv[i + 1] - m_PVTG.Rv[i]) > 0, "Rv must increase with Pdew");
			//Bg must decrease with Pdew
			ASSERT((m_PVTG.SaturatedBg[i + 1] - m_PVTG.SaturatedBg[i]) < 0, "Bg must decrease with Pdew in saturated region");
			//Visc must decrease with Pb
			//ASSERT((m_PVTG.SaturatedViscosity[i + 1] - m_PVTG.SaturatedViscosity[i]) > 0, "Viscosity must increase with Pdew in saturated region");
		}


		//Check for under-saturated branches

		for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
		{

			for (size_t j = 0; j < m_PVTG.UndersaturatedRv[i].size() - 1; ++j)
			{
				//Pressure
				ASSERT((m_PVTG.UndersaturatedRv[i][j + 1] - m_PVTG.UndersaturatedRv[i][j]) < 0, "Rv must decrease in undersaturated region");
			}
		}

	}

	void BlackOil_GasModel::CreateTable(const std::vector<std::vector<double>>& PVT)
	{

		//Count number of Saturated points
		size_t nb_sat = 0;
		for (size_t i = 0; i < PVT.size(); ++i)
		{
			if (PVT[i].size() == 4)
				nb_sat++;
		}
		m_PVTG.NSaturatedPoints = nb_sat;

		//--Store data in a new structure
		m_PVTG.Rv.resize(nb_sat);
		m_PVTG.DewPressure.resize(nb_sat);
		m_PVTG.SaturatedBg.resize(nb_sat);
		m_PVTG.SaturatedViscosity.resize(nb_sat);
		m_PVTG.UndersaturatedRv.resize(nb_sat);
		m_PVTG.UndersaturatedBg.resize(nb_sat);
		m_PVTG.UndersaturatedViscosity.resize(nb_sat);

		auto i_sat = 0;
		for (size_t i = 0; i < PVT.size(); ++i)
		{
			//Saturated part
			m_PVTG.DewPressure[i_sat] = PVT[i][0];
			m_PVTG.Rv[i_sat] = PVT[i][1];
			m_PVTG.SaturatedBg[i_sat] = PVT[i][2];
			m_PVTG.SaturatedViscosity[i_sat] = PVT[i][3];

			
			//--Undersaturated
			//Add Saturated properties - Pressure= P - Pbub
			m_PVTG.UndersaturatedRv[i_sat].push_back(0);
			m_PVTG.UndersaturatedBg[i_sat].push_back(m_PVTG.SaturatedBg[i_sat]);
			m_PVTG.UndersaturatedViscosity[i_sat].push_back(m_PVTG.SaturatedViscosity[i_sat]);


			auto branch_size = 0;
			auto j = i + 1;
			while (PVT[j].size() == 3)
			{
				////Assume dry gas, so ignore it
				/*m_PVTG.UndersaturatedRv[i_sat].push_back(PVT[j][0] - m_PVTG.Rv[i_sat]);
				m_PVTG.UndersaturatedBg[i_sat].push_back(PVT[j][1]);
				m_PVTG.UndersaturatedViscosity[i_sat].push_back(PVT[j][2]);*/
				branch_size++;
				j++;
				if (j == PVT.size())
				{
					break;
				}
			}

			i = i + branch_size;
			i_sat++;
		}



		//Add 1atm value if does not exist yet
		if (!math::isNearlyEqual(m_PVTG.Rv[0],0.))
		{
			auto Pref = 101325.0;
			auto visc = math::LogExtrapolation(m_PVTG.DewPressure[1], m_PVTG.SaturatedViscosity[1], m_PVTG.DewPressure[0], m_PVTG.SaturatedViscosity[0], Pref);

			m_PVTG.Rv.insert(m_PVTG.Rv.begin(), 0.);
			m_PVTG.DewPressure.insert(m_PVTG.DewPressure.begin(), Pref);
			m_PVTG.SaturatedBg.insert(m_PVTG.SaturatedBg.begin(),1.0);
			m_PVTG.SaturatedViscosity.insert(m_PVTG.SaturatedViscosity.begin(), visc);
			m_PVTG.NSaturatedPoints++;

			m_PVTG.UndersaturatedRv.insert(m_PVTG.UndersaturatedRv.begin(), { 0 });
			m_PVTG.UndersaturatedBg.insert(m_PVTG.UndersaturatedBg.begin(), { 1.0 });
			m_PVTG.UndersaturatedViscosity.insert(m_PVTG.UndersaturatedViscosity.begin(), { visc });

		}


		m_PVTG.MaxRelativeRv = 0;
		m_PVTG.MinRelativeRv = 1e8;

		for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
		{
			double maxRv = *(std::max_element(m_PVTG.UndersaturatedRv[i].begin(), m_PVTG.UndersaturatedRv[i].end()));
			double minRv = *(std::min_element(m_PVTG.UndersaturatedRv[i].begin(), m_PVTG.UndersaturatedRv[i].end()));
			m_PVTG.MaxRelativeRv = std::max(maxRv, m_PVTG.MaxRelativeRv);
			m_PVTG.MinRelativeRv = std::min(minRv, m_PVTG.MinRelativeRv);
		}

		//
		max_Pressure = *(std::max_element(m_PVTG.DewPressure.begin(), m_PVTG.DewPressure.end()));
		min_Pressure = *(std::min_element(m_PVTG.DewPressure.begin(), m_PVTG.DewPressure.end()));
	}

	void BlackOil_GasModel::ExtendUnderSaturatedProperties()
	{

		////Extrapolate Undersaturated properties up to max pressure
		//for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
		//{
		//	auto dPext = m_PVTG.MaxRelativeRv - m_PVTG.UndersaturatedRv[i].back();

		//	const auto& Pusat = m_PVTG.UndersaturatedRv[i];
		//	const auto& Bousat = m_PVTG.UndersaturatedBg[i];
		//	const auto& Viscusat = m_PVTG.UndersaturatedViscosity[i];

		//	if (std::fabs(dPext) > 0)
		//	{
		//		auto branch_size = m_PVTG.UndersaturatedRv[i].size();
		//		auto Bo = math::LinearExtrapolation(Pusat[branch_size - 2], Bousat[branch_size - 2], Pusat[branch_size - 1], Bousat[branch_size - 1], m_PVTG.MaxRelativeRv);
		//		auto Visc = math::LinearExtrapolation(Pusat[branch_size - 2], Viscusat[branch_size - 2], Pusat[branch_size - 1], Viscusat[branch_size - 1], m_PVTG.MaxRelativeRv);
		//		m_PVTG.UndersaturatedBg[i].push_back(Bo);
		//		m_PVTG.UndersaturatedViscosity[i].push_back(Visc);
		//		m_PVTG.UndersaturatedRv[i].push_back(m_PVTG.MaxRelativeRv);
		//	}

		//}

	}

	void BlackOil_GasModel::RefineTable(size_t nlevel)
	{

		//std::vector<double> refine_p;
		//for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
		//{
		//	for (size_t j = 0; j < m_PVTG.UndersaturatedRv[i].size(); ++j)
		//	{
		//		refine_p.push_back(m_PVTG.UndersaturatedRv[i][j]);
		//	}
		//}
		//std::sort(refine_p.begin(), refine_p.end());
		//refine_p.erase(std::unique(refine_p.begin(), refine_p.end()), refine_p.end());

		////Linspace
		//double maxval = *std::max_element(refine_p.begin(), refine_p.end());
		//refine_p = math::linspace(0.0, maxval, nlevel);

		//for (size_t i = 0; i < m_PVTG.NSaturatedPoints; ++i)
		//{

		//	//Bo
		//	auto prop_refined = math::Interpolation1(m_PVTG.UndersaturatedRv[i], m_PVTG.UndersaturatedBg[i], refine_p);
		//	m_PVTG.UndersaturatedBg[i].resize(refine_p.size());
		//	m_PVTG.UndersaturatedBg[i] = prop_refined;

		//	//Visc
		//	prop_refined = math::Interpolation1(m_PVTG.UndersaturatedRv[i], m_PVTG.UndersaturatedViscosity[i], refine_p);
		//	m_PVTG.UndersaturatedViscosity[i].resize(refine_p.size());
		//	m_PVTG.UndersaturatedViscosity[i] = prop_refined;


		//	for (size_t j = 0; j < refine_p.size(); ++j)
		//	{
		//		m_PVTG.UndersaturatedRv[i].resize(refine_p.size());
		//		m_PVTG.UndersaturatedRv[i][j] = refine_p[j];
		//	}

		//}

	}

	double BlackOil_GasModel::ComputePdew(double Rv)
	{
		size_t i_lower_branch, i_upper_branch;
		math::FindSurrondingIndex(m_PVTG.Rv, Rv, i_lower_branch, i_upper_branch);
		return math::LinearInterpolation(m_PVTG.Rv[i_lower_branch], m_PVTG.DewPressure[i_lower_branch], m_PVTG.Rv[i_upper_branch], m_PVTG.DewPressure[i_upper_branch], Rv);
	}

	double BlackOil_GasModel::ComputeRv(double Pdew)
	{
		ASSERT((Pdew<max_Pressure)&(Pdew>min_Pressure), "Pressure out of table range");
		size_t i_lower_branch, i_upper_branch;
		math::FindSurrondingIndex(m_PVTG.DewPressure, Pdew, i_lower_branch, i_upper_branch);
		return math::LinearInterpolation(m_PVTG.DewPressure[i_lower_branch], m_PVTG.Rv[i_lower_branch], m_PVTG.DewPressure[i_upper_branch], m_PVTG.Rv[i_upper_branch], Pdew);
	}

	void BlackOil_GasModel::ComputeSaturatedProperties(double Pdew, std::vector<double> composition, double oil_mole_surface_density, double oil_mass_surface_density, PhaseProperties& props_out)
	{
		ASSERT((Pdew < max_Pressure)&(Pdew > min_Pressure), "Pressure out of table range");
		props_out.MoleComposition.value = composition;
		auto Rv = ComputeRv(Pdew);
		double Bg, Visc;
		ComputeSaturatedBgVisc(Rv, Bg, Visc);
		props_out.Viscosity = Visc;
		props_out.MoleDensity.value = ComputeMoleDensity(Rv, Bg, oil_mole_surface_density);
		props_out.MassDensity.value = ComputeMassDensity(Rv, Bg, oil_mass_surface_density);
		props_out.MolecularWeight.value = props_out.MassDensity.value / props_out.MoleDensity.value;
	}

	void BlackOil_GasModel::ComputeUnderSaturatedProperties(double P, std::vector<double> composition, double oil_mole_surface_density, double oil_mass_surface_density, PhaseProperties& props_out)
	{
		ASSERT((P < max_Pressure)&(P > min_Pressure), "Pressure out of table range");
		props_out.MoleComposition.value = composition;
		auto Rv = composition[0] / composition[1] * m_SurfaceMoleDensity/oil_mole_surface_density;
		double Bg, Visc;
		ComputeUndersaturatedBgVisc(Rv, P, Bg, Visc);
		props_out.Viscosity = Visc;
		props_out.MoleDensity.value = ComputeMassDensity(Rv, Bg, oil_mole_surface_density);
		props_out.MassDensity.value = ComputeMassDensity(Rv, Bg, oil_mass_surface_density);
		props_out.MolecularWeight.value = props_out.MassDensity.value / props_out.MoleDensity.value;
	}

	void BlackOil_GasModel::ComputeSaturatedBgVisc(double Rv, double& Bg, double& visc) const
	{
		size_t i_lower_branch, i_upper_branch;
		auto Rs_vec = m_PVTG.Rv;
		auto& Bg_vec = m_PVTG.SaturatedBg;
		auto& visc_vec = m_PVTG.SaturatedViscosity;
		math::FindSurrondingIndex(Rs_vec, Rv, i_lower_branch, i_upper_branch);
		Bg = math::LinearInterpolation(Rv - Rs_vec[i_lower_branch], Rs_vec[i_upper_branch] - Rv, Bg_vec[i_lower_branch], Bg_vec[i_upper_branch]);
		visc = math::LinearInterpolation(Rv - Rs_vec[i_lower_branch], Rs_vec[i_upper_branch] - Rv, visc_vec[i_lower_branch], visc_vec[i_upper_branch]);

	}

	void BlackOil_GasModel::ComputeUndersaturatedBgVisc(double Rv, double P, double& Bg, double& visc)
	{

		//TODO Put real undersaturated properties
		//This function cannot be called
		LOGERROR("cannot be called");

	}

	double BlackOil_GasModel::ComputeMoleDensity(double Rv, double Bg, double surface_oil_mole_density) const
	{
		return 1. / Bg * (m_SurfaceMoleDensity + surface_oil_mole_density * Rv);
	}

	double BlackOil_GasModel::ComputeMassDensity(double Rv, double Bg, double surface_oil_mass_density) const
	{
		return 1. / Bg * (m_SurfaceMassDensity + surface_oil_mass_density * Rv);
	}
}
