/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 TotalEnergies
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
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

  DeadOil_PhaseModel( pvt::PHASE_TYPE type,
                      const std::vector< std::vector< double > > & PVD,
                      double oilSurfaceMassDensity,
                      double oilSurfaceMw );

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
};

}
