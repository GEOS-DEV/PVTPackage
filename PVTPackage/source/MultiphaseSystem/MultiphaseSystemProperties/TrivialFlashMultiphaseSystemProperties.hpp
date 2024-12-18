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

#ifndef PVTPACKAGE_TRIVIALFLASHMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_TRIVIALFLASHMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

#include <map>
#include <vector>

namespace PVTPackage
{

class TrivialFlashMultiphaseSystemProperties final : public CompositionalMultiphaseSystemProperties
{
public:

  TrivialFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                          std::size_t nComponents );

  ~TrivialFlashMultiphaseSystemProperties() override = default;

  void setWaterMoleComposition( std::vector< double > const & moleComposition );

  void setWaterFraction( double const & fraction );
};

}

#endif //PVTPACKAGE_TRIVIALFLASHMULTIPHASESYSTEMPROPERTIES_HPP
