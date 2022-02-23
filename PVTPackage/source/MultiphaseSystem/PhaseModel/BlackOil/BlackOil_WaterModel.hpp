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

#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOilDeadOilProperties.hpp"
#include "PVTWdata.hpp"

#include <vector>

namespace PVTPackage
{

class BlackOil_WaterModel final
{
public:

  BlackOil_WaterModel( const std::vector< double > & PVTW,
                       double waterSurfaceMassDensity,
                       double waterSurfaceMolecularWeight );

  double getSurfaceMolecularWeight() const
  {
    return m_surfaceMolecularWeight;
  }

  BlackOilDeadOilProperties computeProperties( double pressure ) const;

private:

  //PVT data
  PVTWdata m_PVTW{};

  double m_surfaceMassDensity;
  double m_surfaceMoleDensity;
  double m_surfaceMolecularWeight;
};

}
