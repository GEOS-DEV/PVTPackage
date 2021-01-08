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

#ifndef PVTPACKAGE_JSONKEYS_HPP
#define PVTPACKAGE_JSONKEYS_HPP

namespace PVTPackage
{

namespace refactor
{

struct ScalarVectorPropertyAndDerivativesKeys
{
  static constexpr auto VALUE = "value";
  static constexpr auto DP = "dP";
  static constexpr auto DT = "dT";
  static constexpr auto DZ = "dz";
};

struct EnumKeys
{
  static constexpr auto EOS_TYPE_UNKNOWN = "UNKNOWN";
  static constexpr auto EOS_TYPE_REDLICH_KWONG_SOAVE = "REDLICH_KWONG_SOAVE";
  static constexpr auto EOS_TYPE_PENG_ROBINSON = "PENG_ROBINSON";
};

struct PhaseTypeKeys
{
  static constexpr auto UNKNOWN = "UNKNOWN";
  static constexpr auto LIQUID_WATER_RICH = "LIQUID_WATER_RICH";
  static constexpr auto OIL = "OIL";
  static constexpr auto GAS = "GAS";
};

struct FlashKeys
{
  static constexpr auto TYPE = "FLASH_TYPE";

  static constexpr auto BLACK_OIL = "BLACK_OIL";
  static constexpr auto DEAD_OIL = "DEAD_OIL";
  static constexpr auto FREE_WATER = "FREE_WATER";
  static constexpr auto NEGATIVE_TWO_PHASE = "NEGATIVE_TWO_PHASE";
  static constexpr auto TRIVIAL = "TRIVIAL";
};

struct PublicAPIKeys
{
  static constexpr auto INPUT = "INPUT";
  static constexpr auto COMPUTATION = "COMPUTATION";
  static constexpr auto API = "API";
  static constexpr auto OUTPUT = "OUTPUT";

  struct Computation
  {
    static constexpr auto TEMPERATURE = "TEMPERATURE";
    static constexpr auto PRESSURE = "PRESSURE";
    static constexpr auto FEED = "FEED";
  };

  struct BlackOilDeadOil
  {
    static constexpr auto PHASES = "PHASES";
    static constexpr auto TABLE_CONTENTS = "TABLE_CONTENTS";
    static constexpr auto SURFACE_MASS_DENSITIES = "SURFACE_MASS_DENSITIES";
    static constexpr auto SURFACE_MOLECULAR_WEIGHTS = "SURFACE_MOLECULAR_WEIGHTS";
  };

  struct Compositional
  {
    static constexpr auto PHASES = "PHASES";
    static constexpr auto EOS_TYPES = "EOS_TYPES";
    static constexpr auto COMPONENT_NAMES = "COMPONENT_NAMES";
    static constexpr auto COMPONENT_MOLAR_WEIGHTS = "COMPONENT_MOLAR_WEIGHTS";
    static constexpr auto COMPONENT_CRITICAL_TEMPERATURES = "COMPONENT_CRITICAL_TEMPERATURES";
    static constexpr auto COMPONENT_CRITICAL_PRESSURES = "COMPONENT_CRITICAL_PRESSURES";
    static constexpr auto COMPONENT_OMEGAS = "COMPONENT_OMEGAS";
  };

  struct MSP // MultiphaseSystemPropertiesKeys
  {
    static constexpr auto MASS_DENSITY = "MASS_DENSITY";
    static constexpr auto MOLE_DENSITY = "MOLE_DENSITY";
    static constexpr auto MOLECULAR_WEIGHT = "MOLECULAR_WEIGHT";
    static constexpr auto PHASE_MOLE_FRACTION = "PHASE_MOLE_FRACTION";
    static constexpr auto MOLE_COMPOSITION = "MOLE_COMPOSITION";
    static constexpr auto VISCOSITY = "VISCOSITY";
  };
};

} // end of namespace refactor
} // end of namespace PVTPackage

#endif //PVTPACKAGE_JSONKEYS_HPP
