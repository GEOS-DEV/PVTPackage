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
#include "PVTOdata.hpp"

#include "Utils/Assert.hpp"

#include <vector>

namespace PVTPackage
{

class BlackOil_OilModel final
{
public:

  /**
   * @brief Paramter constructor for refactor only
   * FIXME REFACTOR
   */
  BlackOil_OilModel( PVTOdata const & pvto,
                     double minPressure,
                     double maxPressure,
                     double surfaceMassDensity,
                     double surfaceMoleDensity,
                     double surfaceMolecularWeight )
    : m_PVTO( pvto ),
      m_minPressure( minPressure ),
      m_maxPressure( maxPressure ),
      m_surfaceMassDensity( surfaceMassDensity ),
      m_surfaceMoleDensity( surfaceMoleDensity ),
      m_surfaceMolecularWeight( surfaceMolecularWeight )
  { }

  BlackOil_OilModel( const std::vector< std::vector< double > > & PVTO,
                     double oilSurfaceMassDensity,
                     double oilSurfaceMw );

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

  double computeRs( double Pb ) const;

  BlackOilDeadOilProperties computeSaturatedProperties( double Pb,
                                                        double gasMoleSurfaceDensity,
                                                        double gasMassSurfaceDensity ) const;

  BlackOilDeadOilProperties computeUnderSaturatedProperties( double P,
                                                             std::vector< double > composition,
                                                             double gasMoleSurfaceDensity,
                                                             double gasMassSurfaceDensity ) const;

private:

  //PVT data
  PVTOdata m_PVTO;

  double m_minPressure;
  double m_maxPressure;

  double m_surfaceMassDensity;
  double m_surfaceMoleDensity;
  double m_surfaceMolecularWeight;

  double computePb( double Rs ) const;

  void computeSaturatedBoVisc( double Rs,
                               double & Bo,
                               double & visc ) const;

  void computeUndersaturatedBoVisc( double Rs,
                                    double P,
                                    double & Bo,
                                    double & visc ) const;

  double computeMoleDensity( double Rs,
                             double Bo,
                             double surfaceGasMoleDensity ) const;

  double computeMassDensity( double Rs,
                             double Bo,
                             double surfaceGasMassDensity ) const;


  //Functions
  void createTable( const std::vector< std::vector< double > > & PVT );

  void extendUnderSaturatedProperties();

  void createUnderSaturatedProperties();

  void checkTableConsistency() const;

  void refineTable( std::size_t nLevel );

public:
  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  PVTOdata const & getPvto() const
  { return m_PVTO; }

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
