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

#include <vector>

namespace PVTPackage
{

struct PVTOdata
{
  friend class BlackOil_OilModel;

  std::vector< double > Rs;
  std::vector< double > BubblePressure;
  // Saturated
  std::size_t NSaturatedPoints;
  std::vector< double > SaturatedBo;
  std::vector< double > SaturatedViscosity;
  // Unsaturated
  std::vector< std::vector< double > > UndersaturatedPressure;   // Pressure - Pbub -> always start at 0
  std::vector< std::vector< double > > UndersaturatedBo;
  std::vector< std::vector< double > > UndersaturatedViscosity;

private:
  //Pressure
  double MaxRelativePressure;
  double MinRelativePressure;
};

}
