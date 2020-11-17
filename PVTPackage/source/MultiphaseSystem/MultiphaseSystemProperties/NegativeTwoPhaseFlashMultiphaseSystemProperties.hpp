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

#ifndef PVTPACKAGE_NEGATIVETWOPHASEFLASHMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_NEGATIVETWOPHASEFLASHMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/MultiphaseSystemProperties/CompositionalMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"

#include "pvt/pvt.hpp"

#include <map>
#include <vector>

namespace PVTPackage
{

class NegativeTwoPhaseFlashMultiphaseSystemProperties final : public CompositionalMultiphaseSystemProperties
{
public:

  NegativeTwoPhaseFlashMultiphaseSystemProperties( const std::vector< pvt::PHASE_TYPE > & phases,
                                                   const std::vector< pvt::EOS_TYPE > & eosTypes,
                                                   const ComponentProperties & cp );

  ~NegativeTwoPhaseFlashMultiphaseSystemProperties() override = default;

  std::vector< double > const & getOilLnFugacity() const;

  std::vector< double > const & getGasLnFugacity() const;
};

}

#endif //PVTPACKAGE_NEGATIVETWOPHASEFLASHMULTIPHASESYSTEMPROPERTIES_HPP
