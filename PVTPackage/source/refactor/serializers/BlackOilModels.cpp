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

#include "BlackOilModels.hpp"

#include "MultiphaseSystem/PhaseModel/BlackOil/PVTGdata.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/PVTOdata.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/PVTWdata.hpp"

#include "refactor/JsonKeys.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{

using json = nlohmann::json;

void to_json( json & j,
              const PVTGdata & data )
{
  using namespace PVTPackage::refactor;

  j = json{
    { PVTGDataKeys::RV,                       data.Rv },
    { PVTGDataKeys::DEW_PRESSURE,             data.DewPressure },
    { PVTGDataKeys::N_SATURATED_POINTS,       data.NSaturatedPoints },
    { PVTGDataKeys::SATURATED_BG,             data.SaturatedBg },
    { PVTGDataKeys::SATURATED_VISCOSITY,      data.SaturatedViscosity },
    { PVTGDataKeys::UNDERSATURATED_RV,        data.UndersaturatedRv },
    { PVTGDataKeys::UNDERSATURATED_BG,        data.UndersaturatedBg },
    { PVTGDataKeys::UNDERSATURATED_VISCOSITY, data.UndersaturatedViscosity },
    { PVTGDataKeys::MAX_RELATIVE_RV,          data.getMaxRelativeRv() },
    { PVTGDataKeys::MIN_RELATIVE_RV,          data.getMinRelativeRv() }
  };
}

void to_json( json & j,
              const PVTOdata & data )
{
  using namespace PVTPackage::refactor;

  j = json{
    { PVTODataKeys::RS,                       data.Rs },
    { PVTODataKeys::BUBBLE_PRESSURE,          data.BubblePressure },
    { PVTODataKeys::N_SATURATED_POINTS,       data.NSaturatedPoints },
    { PVTODataKeys::SATURATED_BO,             data.SaturatedBo },
    { PVTODataKeys::SATURATED_VISCOSITY,      data.SaturatedViscosity },
    { PVTODataKeys::UNDERSATURATED_PRESSURE,  data.UndersaturatedPressure },
    { PVTODataKeys::UNDERSATURATED_BO,        data.UndersaturatedBo },
    { PVTODataKeys::UNDERSATURATED_VISCOSITY, data.UndersaturatedViscosity },
    { PVTODataKeys::MAX_RELATIVE_PRESSURE,    data.getMaxRelativePressure() },
    { PVTODataKeys::MIN_RELATIVE_PRESSURE,    data.getMinRelativePressure() }
  };
}

void to_json( json & j,
              const PVTWdata & data )
{
  using namespace PVTPackage::refactor;

  j = json{
    { PVTWDataKeys::REFERENCE_PRESSURE, data.ReferencePressure },
    { PVTWDataKeys::BW,                 data.Bw },
    { PVTWDataKeys::COMPRESSIBILITY,    data.Compressibility },
    { PVTWDataKeys::VISCOSITY,          data.Viscosity }
  };
}

void to_json( json & j,
              const BlackOil_GasModel & model )
{
  using namespace PVTPackage::refactor;

  j = json{
    { BlackOilGasModelKeys::PVTG_DATA,                model.getPvtg() },
    { BlackOilGasModelKeys::MIN_PRESSURE,             model.getMinPressure() },
    { BlackOilGasModelKeys::MAX_PRESSURE,             model.getMaxPressure() },
    { BlackOilGasModelKeys::SURFACE_MASS_DENSITY,     model.getSurfaceMassDensity() },
    { BlackOilGasModelKeys::SURFACE_MOLE_DENSITY,     model.getSurfaceMoleDensity() },
    { BlackOilGasModelKeys::SURFACE_MOLECULAR_WEIGHT, model.getSurfaceMolecularWeight() }
  };
}

void to_json( json & j,
              const BlackOil_OilModel & model )
{
  using namespace PVTPackage::refactor;

  j = json{
    { BlackOilOilModelKeys::PVTO_DATA,                model.getPvto() },
    { BlackOilOilModelKeys::MIN_PRESSURE,             model.getMinPressure() },
    { BlackOilOilModelKeys::MAX_PRESSURE,             model.getMaxPressure() },
    { BlackOilOilModelKeys::SURFACE_MASS_DENSITY,     model.getSurfaceMassDensity() },
    { BlackOilOilModelKeys::SURFACE_MOLE_DENSITY,     model.getSurfaceMoleDensity() },
    { BlackOilOilModelKeys::SURFACE_MOLECULAR_WEIGHT, model.getSurfaceMolecularWeight() }
  };
}

void to_json( nlohmann::json & j,
              const BlackOil_WaterModel & model )
{
  using namespace PVTPackage::refactor;

  j = json{
    { BlackOilWaterModelKeys::PVTW_DATA,                model.getPvtw() },
    { BlackOilWaterModelKeys::SURFACE_MASS_DENSITY,     model.getSurfaceMassDensity() },
    { BlackOilWaterModelKeys::SURFACE_MOLE_DENSITY,     model.getSurfaceMoleDensity() },
    { BlackOilWaterModelKeys::SURFACE_MOLECULAR_WEIGHT, model.getSurfaceMolecularWeight() }
  };
}

}