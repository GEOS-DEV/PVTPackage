#include "BlackOilFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include <algorithm>
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"

namespace PVTPackage
{
	BlackOilFlash::BlackOilFlash(): m_OilModel(nullptr), m_GasModel(nullptr), m_WaterModel(nullptr)
	{
		
	}

	void BlackOilFlash::set_PhaseState(MultiphaseSystemProperties& out_variables)
	{

		out_variables.PhaseState = PhaseStateMap.at
		({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
			out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0. ,
			out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value > 0.
			});
	}

	bool BlackOilFlash::ComputeEquilibrium(MultiphaseSystemProperties& out_variables)
	{

		const auto& pressure = out_variables.Pressure;

		// Component index are 0=OIL, 1=GAS, 2=WATER
		auto& zo = out_variables.Feed[0];
		auto& zg = out_variables.Feed[1];
		auto& zw = out_variables.Feed[2];

		const auto& oil_phase_model = dynamic_cast<BlackOil_OilModel*>(out_variables.PhaseModels.at(PHASE_TYPE::OIL));
		const auto& gas_phase_model = dynamic_cast<BlackOil_GasModel*>(out_variables.PhaseModels.at(PHASE_TYPE::GAS));
		const auto& water_phase_model = dynamic_cast<BlackOil_WaterModel*>(out_variables.PhaseModels.at(PHASE_TYPE::LIQUID_WATER_RICH));

		auto& oil_phase_prop = out_variables.PhasesProperties.at(PHASE_TYPE::OIL);
		auto& gas_phase_prop = out_variables.PhasesProperties.at(PHASE_TYPE::GAS);
		auto& water_phase_prop = out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH);

		auto& oil_comp = oil_phase_prop.MoleComposition.value;
		auto& gas_comp = gas_phase_prop.MoleComposition.value;
		auto& water_comp = water_phase_prop.MoleComposition.value;
		auto& oil_comp_dP = oil_phase_prop.MoleComposition.dP;
		auto& gas_comp_dP = gas_phase_prop.MoleComposition.dP;
		auto& water_comp_dP = water_phase_prop.MoleComposition.dP;

		auto& oil_ln_fug = oil_phase_prop.LnFugacityCoefficients.value;
		auto& gas_ln_fug = gas_phase_prop.LnFugacityCoefficients.value;
		auto& water_ln_fug = water_phase_prop.LnFugacityCoefficients.value;
		auto& oil_ln_fug_dP = oil_phase_prop.LnFugacityCoefficients.dP;
		auto& gas_ln_fug_dP = gas_phase_prop.LnFugacityCoefficients.dP;
		auto& water_ln_fug_dP = water_phase_prop.LnFugacityCoefficients.dP;

		auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
		auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;
		auto& water_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value;
				

		//Oil
		 const auto& oil_surface_mole_density =  oil_phase_model->GetSurfaceMoleDensity();
		 const auto& oil_surface_mass_density = oil_phase_model->GetSurfaceMassDensity();
		 //auto Rs_sat = oil_phase_model->ComputeRs(pressure);
		 double Rs_sat, dRS_dp;
		 oil_phase_model->calcRS(pressure, Rs_sat, dRS_dp);


		//Gas
		 const auto& gas_surface_mole_density = gas_phase_model->GetSurfaceMoleDensity();
		 const auto& gas_surface_mass_density = gas_phase_model->GetSurfaceMassDensity();
		 //auto Rv_sat = gas_phase_model->ComputeRv(pressure);
		 double Rv_sat, dRV_dp;
		 gas_phase_model->calcRV(pressure, Rv_sat, dRV_dp);
		 
		 //Phase State - Negative flash type
		 auto Ko = Rv_sat * (oil_surface_mole_density + gas_surface_mole_density * Rs_sat) / (gas_surface_mole_density + oil_surface_mole_density * Rv_sat);
		 auto Kg = (oil_surface_mole_density + gas_surface_mole_density * Rs_sat) / (Rs_sat * (gas_surface_mole_density + oil_surface_mole_density * Rv_sat));
		 auto V = zo / (1. - Kg) + zg / (1. - Ko);

		 double dkodp = 0.0;
		 double dkgdp = 0.0;

		 //bool undersaturated_oil = zg / gas_surface_mole_density - Rs_sat * zo / oil_surface_mole_density;
		 //bool undersaturated_gas = zo / oil_surface_mole_density - Rv_sat * zg / gas_surface_mole_density;

		 if ((V < 1) & (V>0))  //Two-phase or both oil and gas saturated
		 {
			 //Phase Fractions
			 oil_fraction = 1. - V - zw;
			 gas_fraction = V;
			 water_fraction = zw;

			 //oil
			 oil_comp[0] = oil_surface_mole_density / (oil_surface_mole_density + gas_surface_mole_density * Rs_sat);
			 oil_comp[1] = 1. - oil_comp[0];
			 //dxo_dp
			 double dxo_dp= -oil_surface_mole_density / pow(oil_surface_mole_density + gas_surface_mole_density*Rs_sat, 2)
				 *gas_surface_mole_density*dRS_dp;
			 double dxg_dp = -dxo_dp;
			 oil_comp_dP[0] = dxo_dp;
			 oil_comp_dP[1] = dxg_dp;
			 oil_phase_model->ComputeSaturatedProperties(pressure, oil_comp, gas_surface_mole_density, gas_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));

			 //gas
			 gas_comp[1] = gas_surface_mole_density / (gas_surface_mole_density + oil_surface_mole_density * Rv_sat);
			 gas_comp[0] = 1. - gas_comp[1];
			 double dyg_dp = -gas_surface_mole_density / pow(gas_surface_mole_density + oil_surface_mole_density * Rv_sat, 2)
				 *oil_surface_mole_density*dRV_dp;
			 double dyo_dp = -dyg_dp;
			 gas_comp_dP[0] = dyo_dp;
			 gas_comp_dP[1] = dyg_dp;
			 gas_phase_model->ComputeSaturatedProperties(pressure, gas_comp, oil_surface_mole_density, oil_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));
		 
			 // deratives for V
			 dkodp = (dyo_dp- Ko*dxo_dp) / oil_comp[0];
			 dkgdp = (dyg_dp - Kg*dxg_dp) / oil_comp[1];
			 double dVdzo = 1.0 / (1.0 - Kg) - 1.0 / (1.0 - Ko);
			 double dVdp = zo*dkodp / (pow(1.0 - Kg, 2)) - zg*dkgdp / (pow(1.0 - Ko, 2));
			 out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).dP = dVdp;
			 out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).dP = -dVdp;
			 out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).dP = 0.0;
			 std::vector<double> ztemp(2);
			 ztemp[0] = dVdzo; ztemp[1] = -dVdzo;
			 out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).dz = ztemp;
			 ztemp[0] = -dVdzo; ztemp[1] = dVdzo;
			 out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).dz = ztemp;
			 ztemp[0] = 0.0; ztemp[1] = 0.0;
			 out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).dz = ztemp;
			 double xxx = 0.0;
		 }
		 else if (V > 1) //Only gas or undersaturated gas
		 {
			 LOGERROR("Undersaturated gas not supported yet");

			 //Phase Fractions
			 oil_fraction = 0.;
			 gas_fraction = 1.-zw;
			 water_fraction = zw;

			 //gas
			 gas_comp[0] = zo;
			 gas_comp[1] = zg;
			 gas_phase_model->ComputeUnderSaturatedProperties(pressure, gas_comp, oil_surface_mole_density, oil_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));
		 }
		 else //Only oil or undersaturated oil
		 {
			 //Phase Fractions
			 oil_fraction = 1.-zw;
			 gas_fraction = 0.;
			 water_fraction = zw;

			 //oil
			 oil_comp[0] = zo;
			 oil_comp[1] = zg;
			 oil_phase_model->ComputeUnderSaturatedProperties(pressure, oil_comp, gas_surface_mole_density, gas_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));
		 }

		 //Water
		 water_phase_model->ComputeProperties(pressure, out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH));
		 water_comp[0] = 0.;
		 water_comp[1] = 0.;
		 water_comp[2] = zw;

		 std::vector<double> futemp(3);

		 //Fugacity
		 //--oil
		 oil_ln_fug[0] = oil_comp[0] * Ko * pressure;
		 oil_ln_fug[1] = oil_comp[1] * Kg * pressure;
		 oil_ln_fug[2] = water_comp[2] * pressure;
		//derative wrt Pressure
		 futemp[0] = oil_comp_dP[0] * Ko * pressure + oil_comp[0] * dkodp * pressure + oil_comp[0] * Ko;
		 futemp[1] = oil_comp_dP[1] * Kg * pressure + oil_comp[1] * dkgdp * pressure + oil_comp[1] * Kg;
		 futemp[2] = 0.0;
		 oil_ln_fug_dP = futemp;
			 
		 //--gas
		 gas_ln_fug[0] = gas_comp[0] * pressure;
		 gas_ln_fug[1] = gas_comp[1] * pressure;
		 gas_ln_fug[2] = water_comp[2] * pressure;
		 //derative wrt Pressure
		 futemp[0] = gas_comp_dP[0] * pressure + gas_comp[0];
		 futemp[1] = gas_comp_dP[1] * pressure + gas_comp[1];
		 futemp[2] = 0.0;
		 gas_ln_fug_dP = futemp;

		 //--water
		 water_ln_fug[0] = gas_comp[0] * pressure;
		 water_ln_fug[1] = gas_comp[1] * pressure;
		 water_ln_fug[2] = water_comp[2] * pressure;

		 // Compute final phase state
		 set_PhaseState(out_variables);

		 return true;

	}

}


