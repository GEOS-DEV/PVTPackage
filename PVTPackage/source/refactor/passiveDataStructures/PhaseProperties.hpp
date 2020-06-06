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

#ifndef PVTPACKAGE_PDS_PHASEPROPERTIES_HPP
#define PVTPACKAGE_PDS_PHASEPROPERTIES_HPP

#pragma once

#include "refactor/passiveDataStructures/PropertyAndDerivatives.hpp"

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

struct PhaseProperties
{
  ScalarPropertyAndDerivatives< double > MolecularWeight;
  ScalarPropertyAndDerivatives< double > CompressibilityFactor;
  VectorPropertyAndDerivatives< double > MoleComposition;
  VectorPropertyAndDerivatives< double > LnFugacityCoefficients;
  ScalarPropertyAndDerivatives< double > MoleDensity;
  ScalarPropertyAndDerivatives< double > MassDensity;
  double Viscosity;
  double MassEnthalpy;
  double Compressibility;
};

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_PDS_PHASEPROPERTIES_HPP
