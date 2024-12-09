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

#ifndef PVTPACKAGE_FREEWATERFLASHMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_FREEWATERFLASHMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

#include <map>

namespace PVTPackage
{

class FreeWaterFlashMultiphaseSystemProperties final : public CompositionalMultiphaseSystemProperties
{
public:

  FreeWaterFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                            std::size_t nComponents );

  void setWaterMoleComposition( std::vector< double > const & moleComposition );

  void setWaterFraction( double const & fraction );

  std::vector< double > const & getOilLnFugacity() const;

  std::vector< double > const & getGasLnFugacity() const;

  std::vector< double > const & getWaterLnFugacity() const;
};

}

#endif //PVTPACKAGE_FREEWATERFLASHMULTIPHASESYSTEMPROPERTIES_HPP
