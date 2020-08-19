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

#pragma once

#include "MultiphaseSystem/PVTEnums.hpp"

namespace PVTPackage
{

struct MultiphaseSystemProperties;

class Flash
{
public:

  virtual ~Flash() = default;

  Flash() = default;

  bool ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);
  virtual bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) = 0;
  virtual void set_PhaseState(MultiphaseSystemProperties& out_variables) = 0;

protected:

  bool ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables);
};

}
