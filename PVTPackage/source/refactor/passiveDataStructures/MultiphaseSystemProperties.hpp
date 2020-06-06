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

#ifndef PVTPACKAGE_PDS_MULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_PDS_MULTIPHASESYSTEMPROPERTIES_HPP

#pragma once

#include "refactor/passiveDataStructures/PVTEnums.hpp"
#include "refactor/passiveDataStructures/PropertyAndDerivatives.hpp"
#include "refactor/passiveDataStructures/PhaseProperties.hpp"
#include "refactor/passiveDataStructures/PhaseModel.hpp"

#include <unordered_map>
#include <memory>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

struct MultiphaseSystemProperties
{
  double Temperature;
  double Pressure;
  std::vector< double > Feed;
  pds::PHASE_STATE PhaseState;
  std::vector< pds::PHASE_TYPE > PhaseTypes;
  std::unordered_map< pds::PHASE_TYPE, std::shared_ptr< pds::PhaseModel >, pds::EnumClassHash > PhaseModels;
  std::unordered_map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double >, pds::EnumClassHash > PhaseMoleFraction;
  std::unordered_map< pds::PHASE_TYPE, pds::PhaseProperties, pds::EnumClassHash > PhasesProperties;
};

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_PDS_MULTIPHASESYSTEMPROPERTIES_HPP
