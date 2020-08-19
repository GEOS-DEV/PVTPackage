/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#pragma once
#include "Utils/Assert.hpp"
#include  <vector>

struct ComponentProperties
{
  ComponentProperties(size_t n_components, std::vector<std::string> labels, std::vector<double> mw, std::vector<double> tc,
                      std::vector<double> pc, std::vector<double> omega)
    : NComponents(n_components),
      Label(labels),
      Mw(mw),
      Pc(pc),
      Tc(tc),
      Omega(omega),
      BIC(std::vector<std::vector<double>>(NComponents, std::vector<double>(NComponents, 0))),
      VolumeShift(std::vector<std::vector<double>>(NComponents, std::vector<double>(2, 0))), WaterIndex(size_t(-1))
  {
    ASSERT((n_components == labels.size()) && (n_components == mw.size()) && (n_components == tc.size())
           && (n_components == pc.size()) && (n_components == omega.size()), "Dimension Mismatch.");
    for (size_t i = 0; i != NComponents; ++i)
    {
      if (Label[i] == "Water" || Label[i] == "water" || Label[i] == "H2O" || Label[i] == "h2o")
        WaterIndex = i;
    }
  }

  bool operator ==(const ComponentProperties& d) const
  {
    return
      NComponents == d.NComponents&&
      Label == d.Label&&
      Mw == d.Mw&&
      Pc == d.Pc&&
      Tc == d.Tc&&
      Omega == d.Omega&&
      BIC == d.BIC&&
      VolumeShift == d.VolumeShift&&
      WaterIndex == d.WaterIndex;
  }

  const size_t NComponents;
  const std::vector<std::string> Label;
  const std::vector<double> Mw;
  const std::vector<double> Pc;
  const std::vector<double> Tc;
  const std::vector<double> Omega;
  const std::vector<std::vector<double>> BIC;
  const std::vector<std::vector<double>> VolumeShift;
  size_t WaterIndex;
};


