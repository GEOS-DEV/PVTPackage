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
#include "PVTGdata.hpp"

#include "Utils/Assert.hpp"

#include <map>
#include <vector>

namespace PVTPackage
{

class BlackOil_GasModel final
{
public:

  BlackOil_GasModel( const std::vector< std::vector< double > > & PVTG,
                     double gasSurfaceMassDensity,
                     double gasSurfaceMw );

  /**
   * @brief Paramter constructor for refactor only
   * FIXME REFACTOR
   */
  BlackOil_GasModel( const PVTGdata & PVTG,
                     double minPressure,
                     double maxPressure,
                     double surfaceMassDensity,
                     double surfaceMoleDensity,
                     double surfaceMolecularWeight )
    :
    m_PVTG( PVTG ),
    m_minPressure( minPressure ),
    m_maxPressure( maxPressure ),
    m_surfaceMassDensity( surfaceMassDensity ),
    m_surfaceMoleDensity( surfaceMoleDensity ),
    m_surfaceMolecularWeight( surfaceMolecularWeight )
  {
    // Left blank
  }

  const double & getSurfaceMassDensity() const
  {
    return m_surfaceMassDensity;
  }

  const double & getSurfaceMoleDensity() const
  {
    return m_surfaceMoleDensity;
  }

  const double & getSurfaceMolecularWeight() const
  {
    return m_surfaceMolecularWeight;
  }

  double computeRv( double Pdew ) const;

  BlackOilDeadOilProperties computeSaturatedProperties( double Pdew,
                                                        double oilMoleSurfaceDensity,
                                                        double oilMassSurfaceDensity ) const;

private:

  //PVT data
  PVTGdata m_PVTG;

  double m_minPressure;
  double m_maxPressure;

  double m_surfaceMassDensity;
  double m_surfaceMoleDensity;
  double m_surfaceMolecularWeight;

  double computePdew( double Rv ) const;

  void computeSaturatedBgVisc( double Rv,
                               double & Bg,
                               double & viscosity ) const;

  double computeMoleDensity( double Rv,
                             double Bg,
                             double surfaceOilMoleDensity ) const;

  double computeMassDensity( double Rv,
                             double Bg,
                             double surfaceOilMassDensity ) const;

  void createTable( const std::vector< std::vector< double > > & PVT );

  void extendUnderSaturatedProperties();

  void checkTableConsistency() const;

  static void refineTable( std::size_t nLevel );

public:
  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  PVTGdata const & getPvtg() const
  { return m_PVTG; }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getMinPressure() const
  { return m_minPressure; }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getMaxPressure() const
  { return m_maxPressure; }
};

}
