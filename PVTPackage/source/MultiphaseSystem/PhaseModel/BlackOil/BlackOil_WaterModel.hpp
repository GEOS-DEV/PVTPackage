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

  /**
   * @brief Parameter constructor for refactor only
   * FIXME REFACTOR
   */
  BlackOil_WaterModel( const PVTWdata & PVTW,
                       double surfaceMassDensity,
                       double surfaceMoleDensity,
                       double surfaceMolecularWeight )
    :
    m_PVTW( PVTW ),
    m_surfaceMassDensity( surfaceMassDensity ),
    m_surfaceMoleDensity( surfaceMoleDensity ),
    m_surfaceMolecularWeight( surfaceMolecularWeight )
  {
    // Left blank
  }

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

public:
  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getSurfaceMassDensity() const
  {
    return m_surfaceMassDensity;
  }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getSurfaceMoleDensity() const
  {
    return m_surfaceMoleDensity;
  }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  PVTWdata const & getPvtw() const
  {
    return m_PVTW;
  }
};

}
