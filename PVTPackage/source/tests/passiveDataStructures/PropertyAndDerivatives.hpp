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

namespace PVTPackage
{
namespace tests
{
namespace pds
{

template< typename T >
struct ScalarPropertyAndDerivatives
{
  T value;
  T dP;
  T dT;
  std::vector <T> dz;
};

template< typename T >
struct VectorPropertyAndDerivatives
{
  std::vector <T> value;
  std::vector <T> dP;
  std::vector <T> dT;
  std::vector <std::vector< T>> dz;
};

} // end of namespace tests
} // end of namespace pds
} // end of namespace PVTPackage
