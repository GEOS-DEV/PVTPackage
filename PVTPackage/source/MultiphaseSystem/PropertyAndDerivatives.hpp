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

template <typename T>
struct ScalarPropertyAndDerivatives
{
  ScalarPropertyAndDerivatives(std::size_t ncomponents) :value(0),dP(0),dT(0),dz(std::vector<double>(ncomponents, 0)) {}

  T value;
  T dP;
  T dT;
  std::vector<T> dz;
};

template <typename T>
struct VectorPropertyAndDerivatives
{
  VectorPropertyAndDerivatives(std::size_t dim, std::size_t ncomponents)
  :	value(std::vector<T>(dim, 0)),
    dP(std::vector<T>(dim, 0)),
    dT(std::vector<T>(dim, 0)),
    dz(std::vector<std::vector<T>>(dim, std::vector<T>(ncomponents,0))) {}

  std::vector<T> value;
  std::vector<T> dP;
  std::vector<T> dT;
  std::vector<std::vector<T>> dz;


};

}



