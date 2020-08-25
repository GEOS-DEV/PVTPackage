/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once

#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

namespace PVTPackage
{

class CompositionalMultiphaseSystem final: public MultiphaseSystem
{
public:

  CompositionalMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                                std::vector<EOS_TYPE> eos_types, COMPOSITIONAL_FLASH_TYPE flash_type,
                                const ComponentProperties& comp_properties);


  ~CompositionalMultiphaseSystem() override;

};

}
