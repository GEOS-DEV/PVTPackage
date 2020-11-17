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

struct PVTGdata
{
  friend class BlackOil_GasModel;

  std::vector< double > Rv;
  std::vector< double > DewPressure;
  //Saturated
  std::size_t NSaturatedPoints;
  std::vector< double > SaturatedBg;
  std::vector< double > SaturatedViscosity;
  ////Unsaturated
  std::vector< std::vector< double > > UndersaturatedRv;   // always start at 0
  std::vector< std::vector< double > > UndersaturatedBg;
  std::vector< std::vector< double > > UndersaturatedViscosity;

private:
  // Pressure
  double MaxRelativeRv;
  double MinRelativeRv;

public:
  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getMaxRelativeRv() const
  {
    return MaxRelativeRv;
  }

  /**
   * @brief Getter for refactor only
   * FIXME REFACTOR
   */
  double getMinRelativeRv() const
  {
    return MinRelativeRv;
  }

  /**
   * @brief Setter for refactor only
   * FIXME REFACTOR
   */
  void setMaxRelativeRv( double maxRelativeRv )
  {
    MaxRelativeRv = maxRelativeRv;
  }

  /**
   * @brief Setter for refactor only
   * FIXME REFACTOR
   */
  void setMinRelativeRv( double minRelativeRv )
  {
    MinRelativeRv = minRelativeRv;
  }
};

}
