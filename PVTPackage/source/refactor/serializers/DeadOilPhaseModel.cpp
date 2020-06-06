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

#include "DeadOilPhaseModel.hpp"

#include "refactor/JsonKeys.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{

using json = nlohmann::json;

void to_json( json & j,
              const PVDdata & data )
{
  using namespace PVTPackage::refactor;

  j = json{
    { PVDDataKeys::PRESSURE, data.Pressure },
    { PVDDataKeys::N_POINTS, data.NPoints },
    { PVDDataKeys::B, data.B },
    { PVDDataKeys::VISCOSITY, data.Viscosity }
  };
}

void to_json( json & j,
              const DeadOil_PhaseModel & model )
{
  using namespace PVTPackage::refactor;

  j = json{
    { DeadOilModelKeys::PHASE_TYPE_,              model.getType() },
    { DeadOilModelKeys::PVD_DATA,                 model.getPvd() },
    { DeadOilModelKeys::MIN_PRESSURE,             model.getMinPressure() },
    { DeadOilModelKeys::MAX_PRESSURE,             model.getMaxPressure() },
    { DeadOilModelKeys::SURFACE_MASS_DENSITY,     model.getSurfaceMassDensity() },
    { DeadOilModelKeys::SURFACE_MOLE_DENSITY,     model.getSurfaceMoleDensity() },
    { DeadOilModelKeys::SURFACE_MOLECULAR_WEIGHT, model.getSurfaceMolecularWeight() }
  };
}

} // end of namespace PVTPackage
