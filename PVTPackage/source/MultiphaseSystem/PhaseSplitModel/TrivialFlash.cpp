#include "MultiphaseSystem/PhaseSplitModel/TrivialFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <vector>
#include <numeric>
#include "Utils/math.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{

void TrivialFlash::set_PhaseState(MultiphaseSystemProperties & out_variables)
{

  out_variables.PhaseState = PhaseStateMap.at
    ({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
       out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0. ,
       out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value > 0.
     });
}

bool TrivialFlash::ComputeEquilibrium(MultiphaseSystemProperties & out_variables)
{

  const auto& pressure = out_variables.Pressure;
  const auto& temperature = out_variables.Temperature;
  const auto& feed = out_variables.Feed;

  ASSERT(std::fabs(math::sum_array(feed) - 1) < 1e-12, "Feed sum must be 1");

  const auto nbc = m_ComponentsProperties.NComponents;

  auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value = std::vector<double>(nbc,0);
  auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value = std::vector<double>(nbc,0);
  auto& water_comp = out_variables.PhasesProperties.at(PHASE_TYPE::LIQUID_WATER_RICH).MoleComposition.value = std::vector<double>(nbc,0);

  auto KGasOil = ComputeWilsonGasLiquidKvalue(pressure, temperature);
  auto KWaterGas = ComputeWaterGasKvalue(pressure, temperature);
  auto KWaterOil = ComputeWaterOilKvalue(pressure, temperature);

  //Trivial split
  auto& Vo = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value = 0;
  auto& Vg = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value = 0;
  auto& Vw = out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value = 0;

  for (size_t i = 0; i != nbc; ++i)
  {
    if (i != m_ComponentsProperties.WaterIndex)
    {
      //gas-oil
      if (KGasOil[i] > 1)
      {
        gas_comp[i] = feed[i];
        Vg = Vg + feed[i];
      }
      else
      {
        oil_comp[i] = feed[i];
        Vo = Vo + feed[i];
      }
    }
    else
    {
      //water-gas
      if (KWaterGas[i] < 1)
      {
        water_comp[i] = feed[i];
        Vw = Vw + feed[i];
      }
      else
      {
        gas_comp[i] = feed[i];
        Vg = Vg + feed[i];
      }
    }
  }

  //gas_comp = gas_comp / Vg;
  for (size_t i = 0; i != nbc; ++i)
  {
    gas_comp[i] = gas_comp[i] / Vg;
    oil_comp[i] = oil_comp[i] / Vo;
    water_comp[i] = water_comp[i] / Vw;
  }



  //Phase Properties
  for (auto it = out_variables.PhaseModels.begin(); it != out_variables.PhaseModels.end(); ++it)
  {
    auto phase_type = (*it).first;
    auto eos_phase_model = std::dynamic_pointer_cast<CubicEoSPhaseModel>((*it).second);
    auto& comp = out_variables.PhasesProperties.at(phase_type).MoleComposition.value;
    eos_phase_model->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties.at(phase_type));
  }

  //Compute Phase State
  set_PhaseState(out_variables);

  return true;
}

}


