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

#ifndef PVTPACKAGE_BLACKOILDEADOILPROPERTIES_HPP
#define PVTPACKAGE_BLACKOILDEADOILPROPERTIES_HPP

namespace PVTPackage
{

/**
 * @brief Small utility structure for easier output.
 */
struct BlackOilDeadOilProperties
{
  double massDensity;
  double moleDensity;
  double viscosity;

  BlackOilDeadOilProperties( double massDensity_,
                             double moleDensity_,
                             double viscosity_ )
    : massDensity( massDensity_ ),
      moleDensity( moleDensity_ ),
      viscosity( viscosity_ )
  {}
};

}

#endif //PVTPACKAGE_BLACKOILDEADOILPROPERTIES_HPP
