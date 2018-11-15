#include "BlackOilFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include <algorithm>
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"

namespace PVTPackage
{
	BlackOilFlash::BlackOilFlash(): m_OilModel(nullptr), m_GasModel(nullptr),
	                                                                 m_WaterModel(nullptr)
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
		const auto& temperature = out_variables.Temperature;
		// Component index are 0=OIL, 1=GAS, 2=WATER
		auto& zo = out_variables.Feed[0];
		auto& zg = out_variables.Feed[1];

		const auto& oil_phase_model = static_cast<BlackOil_OilModel*>(out_variables.PhaseModels.at(PHASE_TYPE::OIL));
		const auto& gas_phase_model = static_cast<BlackOil_GasModel*>(out_variables.PhaseModels.at(PHASE_TYPE::GAS));


		auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value;
		auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value;

		auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
		auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;

		auto& oil_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).LnFugacityCoefficients.value;
		auto& gas_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).LnFugacityCoefficients.value;



		//Oil
		 const auto& oil_surface_mole_density =  oil_phase_model->GetSurfaceOilMoleDensity();
		 const auto& oil_surface_mass_density = oil_phase_model->GetSurfaceOilMassDensity();
		 auto Rs_sat = oil_phase_model->ComputeRs(pressure);

		//Gas
		 const auto& gas_surface_mole_density = gas_phase_model->GetSurfaceGasMoleDensity();
		 const auto& gas_surface_mass_density = gas_phase_model->GetSurfaceGasMassDensity();
		 auto Rv_sat = gas_phase_model->ComputeRv(pressure);
		 
		 ////Phase state
		 //auto oil_undersaturated = ((zg / gas_surface_mole_density - Rs * zo / oil_surface_mole_density) < 0);
		 //auto gas_undersaturated = ((zo / oil_surface_mole_density - Rv * zg / gas_surface_mole_density) < 0);

		 //Phase State - Negative flash type
		 auto Ko = Rv_sat * (oil_surface_mole_density + gas_surface_mole_density * Rs_sat) / (gas_surface_mole_density + oil_surface_mole_density * Rv_sat);
		 auto Kg = (oil_surface_mole_density + gas_surface_mole_density * Rs_sat) / (Rs_sat * (gas_surface_mole_density + oil_surface_mole_density * Rv_sat));
		 auto V = zo / (1 - Kg) + zg / (1 - Ko);
		 if (V < 1 & V>0)  //Two-phase or both oil and gas saturated
		 {

			 //Phase Fractions
			 oil_fraction = 1. - V;
			 gas_fraction = V;

			 //oil
			 oil_comp[0] = oil_surface_mole_density / (oil_surface_mole_density + gas_surface_mole_density * Rs_sat);
			 oil_comp[1] = 1. - oil_comp[0];
			 oil_phase_model->ComputeSaturatedProperties(pressure, oil_comp, gas_surface_mole_density, gas_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));

			 //gas
			 gas_comp[1] = gas_surface_mole_density / (gas_surface_mole_density + oil_surface_mole_density * Rv_sat);
			 gas_comp[0] = 1. - gas_comp[1];
			 gas_phase_model->ComputeSaturatedProperties(pressure, gas_comp, oil_surface_mole_density, oil_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));
		 }
		 else if (V > 1) //Only gas or undersaturated gas
		 {
			 //Phase Fractions
			 oil_fraction = 0.;
			 gas_fraction = 1.;

			 //gas
			 gas_comp[0] = zo;
			 gas_comp[1] = zg;
			 gas_phase_model->ComputeUnderSaturatedProperties(pressure, gas_comp, oil_surface_mole_density, oil_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));
		 }
		 else //Only oil or undersaturated oil
		 {
			 //Phase Fractions
			 oil_fraction = 1.;
			 gas_fraction = 0.;

			 //oil
			 oil_comp[0] = zo;
			 oil_comp[1] = zg;
			 oil_phase_model->ComputeUnderSaturatedProperties(pressure, oil_comp, gas_surface_mole_density, gas_surface_mass_density, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));
		 }

		 return true;

	}

}


