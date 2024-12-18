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

#ifndef PVTPACKAGE_DEADOILFLASHMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_DEADOILFLASHMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/BlackOilDeadOilMultiphaseSystemProperties.hpp"

#include "MultiphaseSystem/PVTEnums.hpp"

#include "pvt/pvt.hpp"

#include <map>
#include <vector>

namespace PVTPackage
{

class DeadOilFlashMultiphaseSystemProperties : public BlackOilDeadOilMultiphaseSystemProperties
{
public:

  DeadOilFlashMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases );

  double getOilPhaseMoleFraction() const;

  double getGasPhaseMoleFraction() const;

  double getWaterPhaseMoleFraction() const;

  void setFeed( std::vector< double > const & feed ) final;

private:

  // I shall not need this because values are constants
  std::map< pvt::PHASE_TYPE, pvt::VectorPropertyAndDerivatives< double > > m_lnFugacity;
};

}

#endif //PVTPACKAGE_DEADOILFLASHMULTIPHASESYSTEMPROPERTIES_HPP
