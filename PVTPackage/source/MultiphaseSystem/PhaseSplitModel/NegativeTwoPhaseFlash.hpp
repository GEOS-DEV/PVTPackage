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

#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/NegativeTwoPhaseFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

class NegativeTwoPhaseFlash final : private CompositionalFlash
{
public:

  NegativeTwoPhaseFlash( const ComponentProperties & componentProperties )
    : CompositionalFlash( componentProperties )
  { }

  /**
   * @brief Temporary access to component properties used for the computation.
   * @return Reference to const.
   *
   * This member is added for debugging purpose. It should be removed.
   */
  const ComponentProperties & getComponentProperties() const
  {
    return this->m_ComponentsProperties;
  }

  static bool computeEquilibrium( NegativeTwoPhaseFlashMultiphaseSystemProperties & sysProps );
};

}
