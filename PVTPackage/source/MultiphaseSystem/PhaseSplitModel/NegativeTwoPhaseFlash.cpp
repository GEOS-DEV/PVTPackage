/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "MultiphaseSystem/PhaseSplitModel/NegativeTwoPhaseFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include <vector>
#include <numeric>
#include "Utils/math.hpp"
#include "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"

namespace PVTPackage
{

bool NegativeTwoPhaseFlash::ComputeEquilibrium(MultiphaseSystemProperties & out_variables)
{
  // Equilibrium convergence parameters
  const int max_SSI_iterations = 100;
  const double fug_epsilon = 1e-8;

  const auto& pressure = out_variables.Pressure;
  const auto& temperature = out_variables.Temperature;
  const auto& feed = out_variables.Feed;

  ASSERT(std::fabs(math::sum_array(feed) - 1.0) < 1e-12, "Feed sum must be 1");

  const auto nbc = m_ComponentsProperties.NComponents;

  auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value;
  auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value;

  auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
  auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;

  auto& oil_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).LnFugacityCoefficients.value;
  auto& gas_ln_fug = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).LnFugacityCoefficients.value;

  std::vector<double> fug_ratio(nbc);

  auto KGasOil = ComputeWilsonGasLiquidKvalue(pressure, temperature);

  //Check for machine-zero feed values
  const double epsilon = std::numeric_limits<double>::epsilon();
  std::list<size_t> positive_components;
  for (size_t i = 0; i != nbc; ++i)
  {
    if (feed[i] > epsilon)
    {
      positive_components.push_back(i);
    }
  }

  gas_comp.assign(nbc, 0.0);
  oil_comp.assign(nbc, 0.0);

  int total_nb_iter = 0;
  for (int iter = 0; iter < max_SSI_iterations; ++iter)
  {
    // Solve Rachford-Rice Equation
    const double vapor_fraction = SolveRachfordRiceEquation(KGasOil, feed, positive_components);
    gas_fraction = vapor_fraction;
    oil_fraction = 1.0 - gas_fraction;

    // Assign phase compositions
    for (auto ic : positive_components)
    {
      oil_comp[ic] = feed[ic] / (1.0 + vapor_fraction * (KGasOil[ic] - 1.0));
      gas_comp[ic] = KGasOil[ic] * oil_comp[ic];
    }

    oil_comp = math::Normalize(oil_comp);
    gas_comp = math::Normalize(gas_comp);

    // Compute phase fugacity
    for (auto it = out_variables.PhaseModels.begin(); it != out_variables.PhaseModels.end(); ++it)
    {
      auto phase_type = (*it).first;
      auto eos_phase_model = std::dynamic_pointer_cast<CubicEoSPhaseModel>((*it).second);
      auto& comp = out_variables.PhasesProperties.at(phase_type).MoleComposition.value;
      eos_phase_model->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties.at(phase_type));
    }

    // Compute fugacity ratio and check convergence
    bool converged = true;

    for (auto ic : positive_components)
    {
      fug_ratio[ic] = std::exp(oil_ln_fug[ic] - gas_ln_fug[ic]) * oil_comp[ic] / gas_comp[ic];
      if (std::fabs(fug_ratio[ic] - 1.0) > fug_epsilon)
        converged = false;
    }

    if (converged)
      break;

    // Update K-values
    for (auto ic : positive_components)
    {
      KGasOil[ic] *= fug_ratio[ic];
    }

    total_nb_iter = iter;
  }

  // Retrieve physical bounds from negative flash values
  if (gas_fraction <= 0.0 || gas_fraction >= 1.0)
  {
    PHASE_TYPE phase;
    if (gas_fraction >= 1.0)
    {
      gas_fraction = 1.0;
      oil_fraction = 0.0;
      gas_comp = feed;
      phase = PHASE_TYPE::GAS;
    }
    else
    {
      gas_fraction = 0.0;
      oil_fraction = 1.0;
      oil_comp = feed;
      phase = PHASE_TYPE::OIL;
    }

    // Update phase properties since adjusting composition
    auto eos_phase = std::dynamic_pointer_cast<CubicEoSPhaseModel>(out_variables.PhaseModels.at(phase));
    auto & comp = out_variables.PhasesProperties.at(phase).MoleComposition.value;
    eos_phase->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties.at(phase));
  }

  // Compute final phase state
  set_PhaseState(out_variables);

  return total_nb_iter != max_SSI_iterations;
}

}

