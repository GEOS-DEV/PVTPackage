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

#include <vector>
#include "MultiphaseSystem/PVTEnums.hpp"
#include "MultiphaseSystemProperties.hpp"
#include "PhaseModel/PhaseProperties.hpp"

namespace PVTPackage
{

class PhaseModel;
class Flash;

class MultiphaseSystem
{
public:

  enum class State { NOT_INITIALIZED, SUCCESS, NOT_CONVERGED, FATAL_ERROR, NOT_IMPLEMENTED };

  virtual ~MultiphaseSystem() = default;

  MultiphaseSystem(size_t nc, const std::vector<PHASE_TYPE>& phase_types) :
    m_MultiphaseProperties(phase_types, nc),
    m_Flash(nullptr),
    m_StateIndicator(State::NOT_INITIALIZED)
  {
  }

  void Update(double pressure, double temperature, std::vector<double> feed);

  //--Getters
  const MultiphaseSystemProperties& getMultiphaseSystemProperties() const
  {
    return m_MultiphaseProperties;
  }

  const PhaseProperties& getPhaseProperties( PHASE_TYPE phase_type)
  {
    return m_MultiphaseProperties.PhasesProperties.at(phase_type);
  }

  State getState()
  {
    return m_StateIndicator;
  }

protected:

  //Properties
  MultiphaseSystemProperties m_MultiphaseProperties;

  //Flash pointer
  Flash* m_Flash;

  //Success indicator for system state update
  State m_StateIndicator;

};

}
