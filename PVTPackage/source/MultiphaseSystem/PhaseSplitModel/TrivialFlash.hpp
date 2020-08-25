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

class TrivialFlash final : public CompositionalFlash
{
public:

  TrivialFlash(const ComponentProperties& component_properties)
    : CompositionalFlash(component_properties)
  {
  }

  void set_PhaseState(MultiphaseSystemProperties& out_variables) override;

  ~TrivialFlash() override = default;

  bool ComputeEquilibrium(MultiphaseSystemProperties & out_variables) override;

protected:


};

}


