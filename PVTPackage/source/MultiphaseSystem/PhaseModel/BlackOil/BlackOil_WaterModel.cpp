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

#include "BlackOil_WaterModel.hpp"

#include <cmath>

namespace PVTPackage
{

BlackOil_WaterModel::BlackOil_WaterModel( const std::vector< double > & PVTW,
                                          double waterSurfaceMassDensity,
                                          double waterSurfaceMolecularWeight )
  :
  m_surfaceMassDensity( waterSurfaceMassDensity ),
  m_surfaceMolecularWeight( waterSurfaceMolecularWeight )
{
  m_PVTW.ReferencePressure = PVTW[0];
  m_PVTW.Bw = PVTW[1];
  m_PVTW.Compressibility = PVTW[2];
  m_PVTW.Viscosity = PVTW[3];

  //Density
  m_surfaceMoleDensity = m_surfaceMassDensity / m_surfaceMolecularWeight;
}

BlackOilDeadOilProperties BlackOil_WaterModel::computeProperties( double pressure ) const
{
  double const massDensity = m_surfaceMassDensity / ( m_PVTW.Bw * exp( -m_PVTW.Compressibility * ( pressure - m_PVTW.ReferencePressure ) ) );

  return BlackOilDeadOilProperties(
    massDensity,
    massDensity / m_surfaceMolecularWeight,
    m_PVTW.Viscosity
  );
}

}
