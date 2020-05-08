#include "BlackOilFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"
#include <algorithm>
#include <cmath>

namespace PVTPackage
{

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
  auto& zo = out_variables.Feed[static_cast<size_t>(PHASE_TYPE::OIL)];
//  auto& zo = out_variables.Feed[0];
  auto& zg = out_variables.Feed[static_cast<size_t>(PHASE_TYPE::GAS)];
  auto& zw = out_variables.Feed[static_cast<size_t>(PHASE_TYPE::LIQUID_WATER_RICH)];

  const auto& oil_phase_model = std::dynamic_pointer_cast<BlackOil_OilModel>(out_variables.PhaseModels.at(PHASE_TYPE::OIL));
  const auto& gas_phase_model = std::dynamic_pointer_cast<BlackOil_GasModel>(out_variables.PhaseModels.at(PHASE_TYPE::GAS));
  const auto& water_phase_model = std::dynamic_pointer_cast<BlackOil_WaterModel>(out_variables.PhaseModels.at(PHASE_TYPE::LIQUID_WATER_RICH));

  auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value;
  auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value;
  auto& water_comp = out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH).MoleComposition.value;

  auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
  auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;
  auto& water_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value;

  auto& oil_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).LnFugacityCoefficients.value;
  auto& gas_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).LnFugacityCoefficients.value;
  auto& water_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH).LnFugacityCoefficients.value;

  //Oil
  const auto& oil_surface_mole_density =  oil_phase_model->GetSurfaceMoleDensity();
  const auto& oil_surface_mass_density = oil_phase_model->GetSurfaceMassDensity();
  auto Rs_sat = oil_phase_model->ComputeRs(pressure);

  //Gas
  const auto& gas_surface_mole_density = gas_phase_model->GetSurfaceMoleDensity();
  const auto& gas_surface_mass_density = gas_phase_model->GetSurfaceMassDensity();
  auto Rv_sat = gas_phase_model->ComputeRv(pressure);

  //Phase State - Negative flash type
  auto Ko = Rv_sat * (oil_surface_mole_density + gas_surface_mole_density * Rs_sat) / (gas_surface_mole_density + oil_surface_mole_density * Rv_sat);
  auto Kg = (oil_surface_mole_density + gas_surface_mole_density * Rs_sat) / (Rs_sat * (gas_surface_mole_density + oil_surface_mole_density * Rv_sat));
  auto V = zo / (1. - Kg) + zg / (1. - Ko);

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
    oil_comp[2] = 0.;
    oil_phase_model->ComputeSaturatedProperties(pressure, oil_comp,
                                                gas_surface_mole_density, gas_surface_mass_density,
                                                out_variables.PhasesProperties.at(PHASE_TYPE::OIL));

    //gas
    gas_comp[1] = gas_surface_mole_density / (gas_surface_mole_density + oil_surface_mole_density * Rv_sat);
    gas_comp[0] = 1. - gas_comp[1];
    gas_comp[2] = 0.;
    gas_phase_model->ComputeSaturatedProperties(pressure, gas_comp,
                                                oil_surface_mole_density, oil_surface_mass_density,
                                                out_variables.PhasesProperties.at(PHASE_TYPE::GAS));

    //Fugacity
    //--oil
    oil_ln_fug[0] = std::log(oil_comp[0] * Ko * pressure);
    oil_ln_fug[1] = std::log(oil_comp[1] * Kg * pressure);
    oil_ln_fug[2] = std::log(1.);

    //--gas
    gas_ln_fug[0] = std::log(gas_comp[0] * pressure);
    gas_ln_fug[1] = std::log(gas_comp[1] * pressure);
    gas_ln_fug[2] = std::log(1.);

    //--water
    water_ln_fug[0] = std::log(gas_comp[0] * pressure);
    water_ln_fug[1] = std::log(gas_comp[1] * pressure);
    water_ln_fug[2] = std::log(1.);

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
    gas_comp[2] = 0.;
    gas_phase_model->ComputeUnderSaturatedProperties(pressure, gas_comp,
                                                     oil_surface_mole_density, oil_surface_mass_density,
                                                     out_variables.PhasesProperties.at(PHASE_TYPE::GAS));

    //Fugacity
    //--oil
    oil_ln_fug[0] = std::log(1.);
    oil_ln_fug[1] = std::log(1.);
    oil_ln_fug[2] = std::log(1.);

    //--gas
    gas_ln_fug[0] = std::log(1.);
    gas_ln_fug[1] = std::log(1.);
    gas_ln_fug[2] = std::log(1.);

    //--water
    water_ln_fug[0] = std::log(1.);
    water_ln_fug[1] = std::log(1.);
    water_ln_fug[2] = std::log(1.);


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
    oil_comp[2] = 0.;
    oil_phase_model->ComputeUnderSaturatedProperties(pressure, oil_comp,
                                                     gas_surface_mole_density, gas_surface_mass_density,
                                                     out_variables.PhasesProperties.at(PHASE_TYPE::OIL));

    //Fugacity
    //--oil
    oil_ln_fug[0] = std::log(1.);
    oil_ln_fug[1] = std::log(1.);
    oil_ln_fug[2] = std::log(1.);

    //--gas
    gas_ln_fug[0] = std::log(1.);
    gas_ln_fug[1] = std::log(1.);
    gas_ln_fug[2] = std::log(1.);

    //--water
    water_ln_fug[0] = std::log(1.);
    water_ln_fug[1] = std::log(1.);
    water_ln_fug[2] = std::log(1.);

  }

  //Water
  water_phase_model->ComputeProperties(pressure, out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH));
  water_comp[0] = 0.;
  water_comp[1] = 0.;
  water_comp[2] = 1;


  // Compute final phase state
  set_PhaseState(out_variables);

  return true;

}

}
