#include "DeadOilFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/DeadOil_PhaseModel.hpp"
#include <algorithm>
#include <cmath>

namespace PVTPackage
{

void DeadOilFlash::set_PhaseState(MultiphaseSystemProperties& out_variables)
{

  out_variables.PhaseState = PhaseStateMap.at
    ({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
       out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0. ,
       out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value > 0.
     });
}

bool DeadOilFlash::ComputeEquilibrium(MultiphaseSystemProperties& out_variables)
{

  const auto& pressure = out_variables.Pressure;

  // Component index are 0=OIL, 1=GAS, 2=WATER
  auto& zo = out_variables.Feed[0];
  auto& zg = out_variables.Feed[1];
  auto& zw = out_variables.Feed[2];

  const auto& oil_phase_model = std::dynamic_pointer_cast<DeadOil_PhaseModel>(out_variables.PhaseModels.at(PHASE_TYPE::OIL));
  const auto& gas_phase_model = std::dynamic_pointer_cast<DeadOil_PhaseModel>(out_variables.PhaseModels.at(PHASE_TYPE::GAS));
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


  //Two-phase
  //Phase Fractions
  oil_fraction = zo;
  gas_fraction = zg;
  water_fraction = zw;

  //oil
  oil_comp[0] = 1.;
  oil_comp[1] = 0.;
  oil_comp[2] = 0.;
  oil_phase_model->ComputeProperties(pressure, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));

  //gas
  gas_comp[1] = 0.;
  gas_comp[0] = 1.;
  gas_comp[2] = 0.;
  gas_phase_model->ComputeProperties(pressure, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));


  //Water
  water_phase_model->ComputeProperties(pressure, out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH));
  water_comp[0] = 0.;
  water_comp[1] = 0.;
  water_comp[2] = 1.;

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

  // Compute final phase state
  set_PhaseState(out_variables);

  return true;

}

}


