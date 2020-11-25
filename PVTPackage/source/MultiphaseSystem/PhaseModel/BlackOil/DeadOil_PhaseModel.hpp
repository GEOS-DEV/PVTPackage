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
#include "PVDdata.hpp"

#include "Utils/Assert.hpp"

#include "pvt/pvt.hpp"

#include <map>
#include <vector>

namespace PVTPackage
{

class DeadOil_PhaseModel final
{
public:
  /**
   * @brief Parametrized constructof for refactor
   * FIXME REFACTOR
   */
  DeadOil_PhaseModel( pvt::PHASE_TYPE type,
                      PVDdata const & pvd,
                      double minPressure,
                      double maxPressure,
                      double surfaceMassDensity,
                      double surfaceMoleDensity,
                      double surfaceMolecularWeight )
    : m_type( type ),
      m_PVD( pvd ),
      m_minPressure( minPressure ),
      m_maxPressure( maxPressure ),
      m_surfaceMassDensity( surfaceMassDensity ),
      m_surfaceMoleDensity( surfaceMoleDensity ),
      m_surfaceMolecularWeight( surfaceMolecularWeight )
  { }

  DeadOil_PhaseModel( pvt::PHASE_TYPE type,
                      const std::vector< std::vector< double > > & PVD,
                      double oilSurfaceMassDensity,
                      double oilSurfaceMw );

  DeadOil_PhaseModel( pvt::PHASE_TYPE type );
  
  //Getter
  double getSurfaceMassDensity() const
  {
    return m_surfaceMassDensity;
  }

  double getSurfaceMoleDensity() const
  {
    return m_surfaceMoleDensity;
  }

  double getSurfaceMolecularWeight() const
  {
    return m_surfaceMolecularWeight;
  }

  BlackOilDeadOilProperties computeProperties( double pressure ) const;

private:

  //Phase type
  const pvt::PHASE_TYPE m_type;

  //PVT data
  PVDdata m_PVD;

  double m_minPressure{};
  double m_maxPressure{};

  double m_surfaceMassDensity;
  double m_surfaceMoleDensity;
  double m_surfaceMolecularWeight;

  void computeBandVisc( double P,
                        double & B,
                        double & visc ) const;

  double computeMoleDensity( double B ) const;

  double computeMassDensity( double B ) const;

  void createTable( const std::vector< std::vector< double > > & PVD );

  void checkTableConsistency();

public:
  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  pvt::PHASE_TYPE getType() const
  {
    return m_type;
  }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  PVDdata const & getPvd() const
  {
    return m_PVD;
  }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getMinPressure() const
  {
    return m_minPressure;
  }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getMaxPressure() const
  {
    return m_maxPressure;
  }
};

}
