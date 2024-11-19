/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 Total, S.A
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/TrivialFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

class TrivialFlash final : private CompositionalFlash
{
public:

  TrivialFlash( const std::vector< pvt::PHASE_TYPE > & phases,
                const std::vector< pvt::EOS_TYPE > & eosTypes,
                ComponentProperties const & componentProperties );

  bool computeEquilibrium( TrivialFlashMultiphaseSystemProperties & sysProps ) const;
};

}
