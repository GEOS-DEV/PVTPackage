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

#ifndef PVTPACKAGE_BLACKOILFLASHMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_BLACKOILFLASHMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/BlackOilDeadOilMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

#include <vector>

namespace PVTPackage
{

class BlackOilFlashMultiphaseSystemProperties : public BlackOilDeadOilMultiphaseSystemProperties
{
public:

  BlackOilFlashMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases );

  void setOilFraction( double const & fraction );

  void setGasFraction( double const & fraction );

  void setWaterFraction( double const & fraction );

  void setOilMoleComposition( std::vector< double > const & moleComposition );

  void setGasMoleComposition( std::vector< double > const & moleComposition );

private:

  std::map< pvt::PHASE_TYPE, pvt::VectorPropertyAndDerivatives< double > > m_lnFugacity;
};

}

#endif //PVTPACKAGE_BLACKOILFLASHMULTIPHASESYSTEMPROPERTIES_HPP
