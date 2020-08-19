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

#include <vector>

namespace PVTPackage
{

struct PVTGdata
{
  friend class BlackOil_GasModel;

  //--Data
  //
  std::vector<double> Rv;
  std::vector<double> DewPressure;
  //Saturated
  size_t NSaturatedPoints;
  std::vector<double> SaturatedBg;
  std::vector<double> SaturatedViscosity;
  ////Unsaturated
  std::vector<std::vector<double>> UndersaturatedRv;   // always start at 0
  std::vector<std::vector<double>> UndersaturatedBg;
  std::vector<std::vector<double>> UndersaturatedViscosity;

private:
  //Pressure
  double MaxRelativeRv;
  double MinRelativeRv;

};

}
