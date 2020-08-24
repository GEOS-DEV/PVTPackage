/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include <unordered_map>
#include <vector>

namespace PVTPackage
{
class CubicEoSPhaseModel;

struct PhaseModelOutputVariables
{
  PhaseModelOutputVariables(const CubicEoSPhaseModel* phase_model)
  : m_PhaseModel(phase_model),
    CompressibilityFactor(-1),
    MoleDensity(-1), MassDensity(-1)
  {
  }

  const CubicEoSPhaseModel* m_PhaseModel;
  double CompressibilityFactor;
  double MoleDensity;
  double MassDensity;
  double MolecularWeight;
  std::vector<double> LnFugacityCoefficients;

};

}



