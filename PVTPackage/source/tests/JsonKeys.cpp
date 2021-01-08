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

#include "./JsonKeys.hpp"

namespace PVTPackage
{
namespace tests
{

#define DECLARE_STRING( val ) decltype( val ) val

DECLARE_STRING( ScalarVectorPropertyAndDerivativesKeys::VALUE );
DECLARE_STRING( ScalarVectorPropertyAndDerivativesKeys::DP );
DECLARE_STRING( ScalarVectorPropertyAndDerivativesKeys::DT );
DECLARE_STRING( ScalarVectorPropertyAndDerivativesKeys::DZ );

DECLARE_STRING( EnumKeys::EOS_TYPE_UNKNOWN );
DECLARE_STRING( EnumKeys::EOS_TYPE_REDLICH_KWONG_SOAVE );
DECLARE_STRING( EnumKeys::EOS_TYPE_PENG_ROBINSON );

DECLARE_STRING( PhaseTypeKeys::UNKNOWN );
DECLARE_STRING( PhaseTypeKeys::LIQUID_WATER_RICH );
DECLARE_STRING( PhaseTypeKeys::OIL );
DECLARE_STRING( PhaseTypeKeys::GAS );

DECLARE_STRING( FlashKeys::TYPE );
DECLARE_STRING( FlashKeys::BLACK_OIL );
DECLARE_STRING( FlashKeys::DEAD_OIL );
DECLARE_STRING( FlashKeys::FREE_WATER );
DECLARE_STRING( FlashKeys::NEGATIVE_TWO_PHASE );
DECLARE_STRING( FlashKeys::TRIVIAL );

DECLARE_STRING( PublicAPIKeys::INPUT );
DECLARE_STRING( PublicAPIKeys::COMPUTATION );
DECLARE_STRING( PublicAPIKeys::API );
DECLARE_STRING( PublicAPIKeys::OUTPUT );

DECLARE_STRING( PublicAPIKeys::MSP::MASS_DENSITY );
DECLARE_STRING( PublicAPIKeys::MSP::MOLE_DENSITY );
DECLARE_STRING( PublicAPIKeys::MSP::MOLECULAR_WEIGHT );
DECLARE_STRING( PublicAPIKeys::MSP::PHASE_MOLE_FRACTION );
DECLARE_STRING( PublicAPIKeys::MSP::MOLE_COMPOSITION );
DECLARE_STRING( PublicAPIKeys::MSP::VISCOSITY );

#undef DECLARE_STRING

} // end of namespace tests
} // end of namespace PVTPackage
