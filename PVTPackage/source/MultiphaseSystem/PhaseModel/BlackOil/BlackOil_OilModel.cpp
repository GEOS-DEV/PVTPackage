#include "BlackOil_OilModel.hpp"
#include <algorithm>
#include "Utils/math.hpp"
#include "BlackOil_Utils.hpp"
#include <algorithm>    // std::max
#include <complex>
#include "MultiphaseSystem/PhaseModel/PhaseProperties.hpp"

namespace PVTPackage
{
	
	BlackOil_OilModel::BlackOil_OilModel(std::vector<std::vector<double>> PVTO, double oil_surface_mass_density, double oil_surface_mw)
						:	m_SurfaceMassDensity(0), m_SurfaceMoleDensity(0), m_SurfaceMolecularWeight(oil_surface_mw)
	{

		//--Fill table
		CreateTable(PVTO);

		//Check Consistency
		CheckTableConsistency();

		//Create missing undersaturated branches
		CreateUnderSaturatedProperties();

		//Extend existing branches
		ExtendUnderSaturatedProperties();
		
		////Refine Table branches
		RefineTable(100);

		//Check Consistency
		CheckTableConsistency();

		///DEBUG PURPOSE - PLOT table for matlab
		std::ofstream outputFile("PVTO.txt");
		for (size_t i =0 ; i<m_PVTO.NSaturatedPoints; ++i )
		{
			for (size_t j = 0; j < m_PVTO.UndersaturatedPressure[i].size(); ++j)
			{
				outputFile << m_PVTO.Rs[i] << " " << m_PVTO.UndersaturatedPressure[i][j] + m_PVTO.BubblePressure[i] << " " << m_PVTO.UndersaturatedBo[i][j] << " " << m_PVTO.UndersaturatedViscosity[i][j] << "\n";
			}
		}


		//Density
		m_SurfaceMassDensity = oil_surface_mass_density;
		m_SurfaceMoleDensity = m_SurfaceMassDensity / m_SurfaceMolecularWeight;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	void BlackOil_OilModel::CheckTableConsistency()
	{
		//--Check for the presence of one bubble point
		ASSERT(m_PVTO.UndersaturatedPressure[m_PVTO.NSaturatedPoints-1].size() > 1, "At least one bubble pressure is required");


		//Check for saturated region
		for (size_t i = 0; i < m_PVTO.NSaturatedPoints - 1; ++i)
		{
			//Rs must increase with Pb
			ASSERT((m_PVTO.Rs[i + 1] - m_PVTO.Rs[i])>0, "Rs must increase with Pb");
			//Bo must increase with Pb
			ASSERT((m_PVTO.SaturatedBo[i + 1] - m_PVTO.SaturatedBo[i])>0, "Bo must increase with Pb in saturated region");
			//Visc must decrease with Pb
			ASSERT((m_PVTO.SaturatedViscosity[i + 1] - m_PVTO.SaturatedViscosity[i])<0, "Viscosity must increase with Pb in saturated region");
		}


		//Check for under-saturated branches

		for (size_t i = 0; i < m_PVTO.NSaturatedPoints; ++i)
		{
		
			for (size_t j = 0; j < m_PVTO.UndersaturatedPressure[i].size() - 1; ++j)
			{
				//Pressure
				ASSERT((m_PVTO.UndersaturatedPressure[i][j + 1] - m_PVTO.UndersaturatedPressure[i][j])>0, "P must decrease in undersaturated region");
				//Bo must decrease with P
				ASSERT((m_PVTO.UndersaturatedBo[i][j + 1] - m_PVTO.UndersaturatedBo[i][j])<0, "Bo must decrease with P in undersaturated region");
				//Visc must increase with Pb
				ASSERT((m_PVTO.UndersaturatedViscosity[i][j + 1] - m_PVTO.UndersaturatedViscosity[i][j])>0, "Viscosity must increase with P in undersaturated region");
			}
		}

	}

	void BlackOil_OilModel::CreateTable(const std::vector<std::vector<double>>& PVT)
	{

		//Count number of Saturated points
		size_t nb_sat=0;
		for (size_t i = 0; i < PVT.size(); ++i)
		{
			if (PVT[i].size() == 4)
				nb_sat++;
		}
		m_PVTO.NSaturatedPoints = nb_sat;

		//--Store data in a new structure
		m_PVTO.Rs.resize(nb_sat);
		m_PVTO.BubblePressure.resize(nb_sat);
		m_PVTO.SaturatedBo.resize(nb_sat);
		m_PVTO.SaturatedViscosity.resize(nb_sat);
		m_PVTO.UndersaturatedPressure.resize(nb_sat);
		m_PVTO.UndersaturatedBo.resize(nb_sat);
		m_PVTO.UndersaturatedViscosity.resize(nb_sat);

		auto i_sat = 0;
		for (size_t i = 0; i < PVT.size(); ++i)
		{
			//Saturated part
			m_PVTO.Rs[i_sat]=PVT[i][0];
			m_PVTO.BubblePressure[i_sat]=PVT[i][1];
			m_PVTO.SaturatedBo[i_sat]=PVT[i][2];
			m_PVTO.SaturatedViscosity[i_sat]=PVT[i][3];

			//--Undersaturated
			//Add Saturated properties - Pressure= P - Pbub
			m_PVTO.UndersaturatedPressure[i_sat].push_back(0);
			m_PVTO.UndersaturatedBo[i_sat].push_back(m_PVTO.SaturatedBo[i_sat]);
			m_PVTO.UndersaturatedViscosity[i_sat].push_back(m_PVTO.SaturatedViscosity[i_sat]);


			auto branch_size = 0;
			auto j = i + 1;
			while (PVT[j].size() == 3)
			{
				m_PVTO.UndersaturatedPressure[i_sat].push_back(PVT[j][0]- m_PVTO.BubblePressure[i_sat]);
				m_PVTO.UndersaturatedBo[i_sat].push_back(PVT[j][1]);
				m_PVTO.UndersaturatedViscosity[i_sat].push_back(PVT[j][2]);
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
		if (!math::isNearlyEqual(m_PVTO.Rs[0],0.))
		{
			auto Pref = 101325.0;
			auto visc = math::LogExtrapolation(m_PVTO.BubblePressure[1],m_PVTO.SaturatedViscosity[1], m_PVTO.BubblePressure[0], m_PVTO.SaturatedViscosity[0], Pref);

			m_PVTO.Rs.insert(m_PVTO.Rs.begin(), 0.);
			m_PVTO.BubblePressure.insert(m_PVTO.BubblePressure.begin(), Pref);
			m_PVTO.SaturatedBo.insert(m_PVTO.SaturatedBo.begin(), 1.0);
			m_PVTO.SaturatedViscosity.insert(m_PVTO.SaturatedViscosity.begin(), visc);
			m_PVTO.NSaturatedPoints++;

			m_PVTO.UndersaturatedPressure.insert(m_PVTO.UndersaturatedPressure.begin(),{ 0 });
			m_PVTO.UndersaturatedBo.insert(m_PVTO.UndersaturatedBo.begin(),{ 1.0 });
			m_PVTO.UndersaturatedViscosity.insert(m_PVTO.UndersaturatedViscosity.begin(),{ visc });

		}

		m_PVTO.MaxRelativePressure = 0;
		m_PVTO.MinRelativePressure = 1e8;

		for (size_t i = 0; i<m_PVTO.NSaturatedPoints; ++i)
		{
			double maxP = *(std::max_element(m_PVTO.UndersaturatedPressure[i].begin(), m_PVTO.UndersaturatedPressure[i].end()));
			double minP = *(std::min_element(m_PVTO.UndersaturatedPressure[i].begin(), m_PVTO.UndersaturatedPressure[i].end()));
			m_PVTO.MaxRelativePressure = std::max(maxP, m_PVTO.MaxRelativePressure);
			m_PVTO.MinRelativePressure = std::min(minP, m_PVTO.MinRelativePressure);
		}

		//
		max_Pressure = *(std::max_element(m_PVTO.BubblePressure.begin(), m_PVTO.BubblePressure.end()));
		min_Pressure = *(std::min_element(m_PVTO.BubblePressure.begin(), m_PVTO.BubblePressure.end()));


	}

	void BlackOil_OilModel::ExtendUnderSaturatedProperties()
	{
		
		//Extrapolate Undersaturated properties up to max pressure
		for (size_t i = 0; i < m_PVTO.NSaturatedPoints; ++i)
		{			
			auto dPext = m_PVTO.MaxRelativePressure - m_PVTO.UndersaturatedPressure[i].back();

			const auto& Pusat = m_PVTO.UndersaturatedPressure[i];
			const auto& Bousat = m_PVTO.UndersaturatedBo[i];
			const auto& Viscusat = m_PVTO.UndersaturatedViscosity[i];

			if (std::fabs(dPext) > 0)
			{
				auto branch_size = m_PVTO.UndersaturatedPressure[i].size();
				auto Bo = math::LinearExtrapolation(Pusat[branch_size - 2], Bousat[branch_size - 2], Pusat[branch_size - 1], Bousat[branch_size - 1], m_PVTO.MaxRelativePressure);
				auto Visc = math::LinearExtrapolation(Pusat[branch_size - 2], Viscusat[branch_size - 2], Pusat[branch_size - 1], Viscusat[branch_size - 1], m_PVTO.MaxRelativePressure);
				m_PVTO.UndersaturatedBo[i].push_back(Bo);
				m_PVTO.UndersaturatedViscosity[i].push_back(Visc);
				m_PVTO.UndersaturatedPressure[i].push_back(m_PVTO.MaxRelativePressure);
			}
			
		}

	}

	void BlackOil_OilModel::CreateUnderSaturatedProperties()
	{

		auto upper_branch_index = m_PVTO.NSaturatedPoints - 1;

		//--Construct undersaturated branches by interpolation, start from second highest Rs

		for (size_t i_current = m_PVTO.NSaturatedPoints - 2; i_current-- > 0; )
		{
			auto lower_branch_index = i_current;
			if (m_PVTO.UndersaturatedPressure[i_current].size() == 1)	//Only Saturated part is present then create missing part
			{
				for (size_t i_lower = i_current; i_lower-- > 0; )
				{
					if (m_PVTO.UndersaturatedPressure[i_lower].size() > 1) // Seek for lower under-saturated branch
					{
						lower_branch_index = i_lower;
						break;
					}
				}

				if (lower_branch_index == i_current)
				{
					lower_branch_index = upper_branch_index;
				}

				auto dRs_up = std::fabs(m_PVTO.Rs[upper_branch_index] - m_PVTO.Rs[i_current]);
				auto dRs_dn = std::fabs(m_PVTO.Rs[i_current] - m_PVTO.Rs[lower_branch_index]);

				//Generate merge of pressures
				std::vector<double> p_target;
				for (size_t i = 0; i < m_PVTO.UndersaturatedPressure[upper_branch_index].size(); ++i)
				{
					p_target.push_back(m_PVTO.UndersaturatedPressure[upper_branch_index][i]);
				}
				for (size_t i = 0; i < m_PVTO.UndersaturatedPressure[lower_branch_index].size(); ++i)
				{
					p_target.push_back(m_PVTO.UndersaturatedPressure[lower_branch_index][i]);
				}
				std::sort(p_target.begin(), p_target.end());
				p_target.erase(std::unique(p_target.begin(), p_target.end()), p_target.end());

				//Shit pressures up and down
				std::vector<double> x_up(m_PVTO.UndersaturatedPressure[upper_branch_index].size()), x_dn(m_PVTO.UndersaturatedPressure[lower_branch_index].size());
				for (size_t i = 0; i < m_PVTO.UndersaturatedPressure[upper_branch_index].size(); ++i)
				{
					x_up[i] = m_PVTO.UndersaturatedPressure[upper_branch_index][i];
				}

				for (size_t i = 0; i < m_PVTO.UndersaturatedPressure[lower_branch_index].size(); ++i)
				{
					x_dn[i] = m_PVTO.UndersaturatedPressure[lower_branch_index][i];
				}

				//Create branch
				for (size_t i = 1; i < p_target.size(); ++i)
				{

					m_PVTO.UndersaturatedPressure[i_current].push_back(p_target[i]);
					//Bo
					auto Bo_up = m_PVTO.UndersaturatedBo[upper_branch_index];
					auto Bo_dn = m_PVTO.UndersaturatedBo[lower_branch_index];
					auto Bo_interp_up = math::Interpolation1(x_up, Bo_up, p_target);
					auto Bo_interp_dn = math::Interpolation1(x_dn, Bo_dn, p_target);
					auto Bo_slope_up = (Bo_interp_up[i] - Bo_interp_up[i - 1]) / (p_target[i] - p_target[i - 1]);
					auto Bo_slope_dn = (Bo_interp_dn[i] - Bo_interp_dn[i - 1]) / (p_target[i] - p_target[i - 1]);
					auto Bo_slope = (1 / dRs_up * Bo_slope_up + 1 / dRs_dn * Bo_slope_dn) / (1 / dRs_up + 1 / dRs_dn);
					m_PVTO.UndersaturatedBo[i_current].push_back(m_PVTO.UndersaturatedBo[i_current][i - 1] + Bo_slope * (p_target[i] - p_target[i - 1]));

					//Visc
					auto Visc_up = m_PVTO.UndersaturatedViscosity[upper_branch_index];
					auto Visc_dn = m_PVTO.UndersaturatedViscosity[lower_branch_index];
					auto Visc_interp_up = math::Interpolation1(x_up, Visc_up, p_target);
					auto Visc_interp_dn = math::Interpolation1(x_dn, Visc_dn, p_target);
					auto Visc_slope_up = (Visc_interp_up[i] - Visc_interp_up[i - 1]) / (p_target[i] - p_target[i - 1]);
					auto Visc_slope_dn = (Visc_interp_dn[i] - Visc_interp_dn[i - 1]) / (p_target[i] - p_target[i - 1]);
					auto Visc_slope = (1 / dRs_up * Visc_slope_up + 1 / dRs_dn * Visc_slope_dn) / (1 / dRs_up + 1 / dRs_dn);
					m_PVTO.UndersaturatedViscosity[i_current].push_back(m_PVTO.UndersaturatedViscosity[i_current][i - 1] + Visc_slope * (p_target[i] - p_target[i - 1]));
				}

			}

			upper_branch_index = i_current;
		}

	}

	void BlackOil_OilModel::RefineTable(size_t nlevel)
	{

		std::vector<double> refine_p;
		for (size_t i = 0; i<m_PVTO.NSaturatedPoints; ++i)
		{
			for (size_t j = 0; j < m_PVTO.UndersaturatedPressure[i].size(); ++j)
			{
				refine_p.push_back(m_PVTO.UndersaturatedPressure[i][j]);
			}
		}
		std::sort(refine_p.begin(), refine_p.end());
		refine_p.erase(std::unique(refine_p.begin(), refine_p.end()), refine_p.end());

		//Linspace
		double maxval = *std::max_element(refine_p.begin(), refine_p.end());
		refine_p = math::linspace(0.0, maxval, nlevel);

		for (size_t i = 0;  i<m_PVTO.NSaturatedPoints; ++i)
		{
			
				//Bo
				auto prop_refined = math::Interpolation1(m_PVTO.UndersaturatedPressure[i],  m_PVTO.UndersaturatedBo[i], refine_p);
				m_PVTO.UndersaturatedBo[i].resize(refine_p.size());
				m_PVTO.UndersaturatedBo[i] = prop_refined;

				//Visc
				prop_refined = math::Interpolation1(m_PVTO.UndersaturatedPressure[i], m_PVTO.UndersaturatedViscosity[i], refine_p);
				m_PVTO.UndersaturatedViscosity[i].resize(refine_p.size());
				m_PVTO.UndersaturatedViscosity[i] = prop_refined;
		

			for (size_t j = 0; j < refine_p.size(); ++j)
			{
				m_PVTO.UndersaturatedPressure[i].resize(refine_p.size());
				m_PVTO.UndersaturatedPressure[i][j] = refine_p[j] ;
			}

		}

	}

	double BlackOil_OilModel::ComputePb(double Rs)
	{
		size_t i_lower_branch, i_upper_branch;
		math::FindSurrondingIndex(m_PVTO.Rs, Rs, i_lower_branch, i_upper_branch);
		return math::LinearInterpolation(m_PVTO.Rs[i_lower_branch], m_PVTO.BubblePressure[i_lower_branch], m_PVTO.Rs[i_upper_branch], m_PVTO.BubblePressure[i_upper_branch], Rs);
	}

	double BlackOil_OilModel::ComputeRs(double Pb)
	{
		ASSERT((Pb < max_Pressure)&(Pb > min_Pressure), "Pressure out of table range");
		size_t i_lower_branch, i_upper_branch;
		math::FindSurrondingIndex(m_PVTO.BubblePressure, Pb, i_lower_branch, i_upper_branch);
		return math::LinearInterpolation(m_PVTO.BubblePressure[i_lower_branch], m_PVTO.Rs[i_lower_branch], m_PVTO.BubblePressure[i_upper_branch], m_PVTO.Rs[i_upper_branch], Pb);
	}

	void BlackOil_OilModel::ComputeSaturatedProperties(double Pb, std::vector<double> composition, double gas_mole_surface_density, double gas_mass_surface_density, PhaseProperties& props_out)
	{
		ASSERT((Pb < max_Pressure)&(Pb > min_Pressure), "Pressure out of table range");
		props_out.MoleComposition.value = composition;
		auto Rs = ComputeRs(Pb);
		double Bo, Visc;
		ComputeSaturatedBoVisc(Rs, Bo, Visc);
		props_out.Viscosity = Visc;
		props_out.MoleDensity.value = ComputeMoleDensity(Rs, Bo, gas_mole_surface_density);
		props_out.MassDensity.value = ComputeMassDensity(Rs, Bo, gas_mass_surface_density);
		props_out.MolecularWeight.value = props_out.MassDensity.value / props_out.MoleDensity.value;
	}

	void BlackOil_OilModel::ComputeUnderSaturatedProperties(double P, std::vector<double> composition, double gas_mole_surface_density, double gas_mass_surface_density, PhaseProperties& props_out)
	{
		ASSERT((P<max_Pressure)&(P>min_Pressure), "Pressure out of table range");
		props_out.MoleComposition.value = composition;
		auto Rs = (composition[1] / gas_mole_surface_density) /  (composition[0] / m_SurfaceMoleDensity) ;
		double Bo, Visc;
		ComputeUndersaturatedBoVisc(Rs,P,Bo, Visc);
		props_out.Viscosity = Visc;
		props_out.MoleDensity.value = ComputeMoleDensity(Rs, Bo, gas_mole_surface_density);
		props_out.MassDensity.value = ComputeMassDensity(Rs, Bo, gas_mass_surface_density);
		props_out.MolecularWeight.value = props_out.MassDensity.value / props_out.MoleDensity.value;

	}

	void BlackOil_OilModel::ComputeSaturatedBoVisc(double Rs, double& Bo, double& visc) const
	{
		size_t i_lower_branch, i_upper_branch;
		auto Rs_vec = m_PVTO.Rs;
		auto& Bo_vec = m_PVTO.SaturatedBo;
		auto& visc_vec = m_PVTO.SaturatedViscosity;
		math::FindSurrondingIndex(Rs_vec, Rs, i_lower_branch, i_upper_branch);
		Bo = math::LinearInterpolation(Rs - Rs_vec[i_lower_branch], Rs_vec[i_upper_branch] - Rs, Bo_vec[i_lower_branch], Bo_vec[i_upper_branch]);
		visc = math::LinearInterpolation(Rs - Rs_vec[i_lower_branch], Rs_vec[i_upper_branch] - Rs, visc_vec[i_lower_branch], visc_vec[i_upper_branch]);
	}

	void BlackOil_OilModel::ComputeUndersaturatedBoVisc(double Rs, double P, double& Bo, double& visc)
	{
		ASSERT((P<max_Pressure)&(P>min_Pressure), "Pressure out of table range");
		size_t i_lower_branch, i_upper_branch;
		auto& Rs_vec = m_PVTO.Rs;
		math::FindSurrondingIndex(Rs_vec, Rs, i_lower_branch, i_upper_branch);

		auto Pbub = math::LinearInterpolation(m_PVTO.Rs[i_lower_branch], m_PVTO.BubblePressure[i_lower_branch], m_PVTO.Rs[i_upper_branch], m_PVTO.BubblePressure[i_upper_branch],Rs);

		ASSERT(P >= Pbub, "Pressure must be greater than Pb for undersaturated properties calculation");

		auto Prel = P - Pbub;

		auto dRs_up = std::abs(m_PVTO.Rs[i_upper_branch] - Rs);
		auto dRs_dn = std::abs(Rs - m_PVTO.Rs[i_lower_branch]);

		auto& Pup = m_PVTO.UndersaturatedPressure[i_upper_branch];
		auto& Pdn = m_PVTO.UndersaturatedPressure[i_lower_branch];

		size_t i_lower_P, i_upper_P;
		math::FindSurrondingIndex(Pup, Prel, i_lower_P, i_upper_P);

		//Bo
		auto Bo_up = m_PVTO.UndersaturatedBo[i_upper_branch];
		auto Bo_dn = m_PVTO.UndersaturatedBo[i_lower_branch];
		auto Bo_interp_dn = math::LinearInterpolation(Pdn[i_lower_P], Bo_dn[i_lower_P], Pdn[i_upper_P], Bo_dn[i_upper_P], Prel);
		auto Bo_interp_up = math::LinearInterpolation(Pup[i_lower_P], Bo_up[i_lower_P], Pup[i_upper_P], Bo_up[i_upper_P], Prel);
		Bo = math::LinearInterpolation(dRs_dn, dRs_up, Bo_interp_dn, Bo_interp_up);

		//Visc
		auto Visc_up = m_PVTO.UndersaturatedViscosity[i_upper_branch];
		auto Visc_dn = m_PVTO.UndersaturatedViscosity[i_lower_branch];
		auto Visc_interp_dn = math::LinearInterpolation(Pdn[i_lower_P], Visc_dn[i_lower_P], Pdn[i_upper_P], Visc_dn[i_upper_P], Prel);
		auto Visc_interp_up = math::LinearInterpolation(Pup[i_lower_P], Visc_up[i_lower_P], Pup[i_upper_P], Visc_up[i_upper_P], Prel);
		visc = math::LinearInterpolation(dRs_dn, dRs_up, Visc_interp_dn, Visc_interp_up);

	}

	double BlackOil_OilModel::ComputeMoleDensity(double Rs, double Bo, double surface_gas_mole_density ) const
	{
		return 1. / Bo*(m_SurfaceMoleDensity + surface_gas_mole_density * Rs);
	}

	double BlackOil_OilModel::ComputeMassDensity(double Rs, double Bo, double surface_gas_mass_density) const
	{
		return 1. / Bo * (m_SurfaceMassDensity + surface_gas_mass_density * Rs);
	}
}
