/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"

namespace PVTPackage
{

class NegativeTwoPhaseFlash final : public CompositionalFlash
{
public:

  NegativeTwoPhaseFlash(const ComponentProperties& component_properties)
    : CompositionalFlash(component_properties)
  {
  }

  ~NegativeTwoPhaseFlash() override = default;


  void set_PhaseState(MultiphaseSystemProperties& out_variables) override
  {

    out_variables.PhaseState = PhaseStateMap.at
      ({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
         out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0.,
         false
       });
  }

  bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) override;

protected:


};

}


