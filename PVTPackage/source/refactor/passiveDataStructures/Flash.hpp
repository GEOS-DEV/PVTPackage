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

#ifndef GEOSX_FLASH_HPP
#define GEOSX_FLASH_HPP

#include "refactor/passiveDataStructures/ComponentProperties.hpp"

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

struct BlackOilFlash
{

};

struct DeadOilFlash
{

};

struct CompositionalFlash
{
  pds::ComponentProperties componentsProperties;
};

struct FreeWaterFlash: public CompositionalFlash
{
  std::size_t waterIndex;
};

struct NegativeTwoPhaseFlash: public CompositionalFlash
{

};

struct TrivialFlash: public CompositionalFlash
{

};

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //GEOSX_FLASH_HPP
