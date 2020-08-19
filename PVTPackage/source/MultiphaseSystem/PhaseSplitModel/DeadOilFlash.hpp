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

#include "Flash.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <unordered_map>
#include <list>
#include "Flash.hpp"

namespace PVTPackage
{

struct MultiphaseSystemProperties;
class PhaseModel;

class DeadOilFlash : public Flash
{
public:

  DeadOilFlash() = default;
  bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;
  void set_PhaseState(MultiphaseSystemProperties& out_variables) override;

  ~DeadOilFlash() override = default;


};

}
