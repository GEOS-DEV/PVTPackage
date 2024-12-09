/*	
 * ------------------------------------------------------------------------------------------------------------	
 * SPDX-License-Identifier: LGPL-2.1-only	
 *	
 * Copyright (c) 2018-2024 Lawrence Livermore National Security LLC	
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University	
 * Copyright (c) 2018-2024 TotalEnergies	
 * Copyright (c) 2019-     GEOS/GEOSX Contributors	
 * All right reserved	
 *	
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMØENTS files for details.
 * ------------------------------------------------------------------------------------------------------------	
 */

#include "./MultiphaseSystemProperties.hpp"
#include "../passiveDataStructures/PVTEnums.hpp"

#include "../JsonKeys.hpp"

namespace pvt
{

NLOHMANN_JSON_SERIALIZE_ENUM( pvt::PHASE_TYPE, {
  { pvt::PHASE_TYPE::UNKNOWN, PVTPackage::tests::PhaseTypeKeys::UNKNOWN },
  { pvt::PHASE_TYPE::LIQUID_WATER_RICH, PVTPackage::tests::PhaseTypeKeys::LIQUID_WATER_RICH },
  { pvt::PHASE_TYPE::OIL, PVTPackage::tests::PhaseTypeKeys::OIL },
  { pvt::PHASE_TYPE::GAS, PVTPackage::tests::PhaseTypeKeys::GAS }
} )

void to_json( nlohmann::json & j,
              const ScalarPropertyAndDerivatives< double > & s )
{
  using Keys = PVTPackage::tests::ScalarVectorPropertyAndDerivativesKeys;
  j = {
    { Keys::VALUE, s.value },
    { Keys::DP,    s.dP },
    { Keys::DT,    s.dT },
    { Keys::DZ,    s.dz }
  };
}

void to_json( nlohmann::json & j,
              const VectorPropertyAndDerivatives< double > & s )
{
  using Keys = PVTPackage::tests::ScalarVectorPropertyAndDerivativesKeys;
  j = {
    { Keys::VALUE, s.value },
    { Keys::DP,    s.dP },
    { Keys::DT,    s.dT },
    { Keys::DZ,    s.dz }
  };
}

void to_json( nlohmann::json & j,
              const MultiphaseSystemProperties & msp )
{
  using Keys = PVTPackage::tests::PublicAPIKeys::MSP;

  // The dataProvider takes a phase and returns the data accordingly.
  // This lambda function is a generic coding hack because data can be scalar or vector.
  auto f = []( const auto & dataProvider )
  {
    nlohmann::json tmp;
    for( PHASE_TYPE phase:{ PHASE_TYPE::OIL, PHASE_TYPE::GAS, PHASE_TYPE::LIQUID_WATER_RICH } )
    {
      // Small hack to have my enum properly considered by the nlohmann lib as a json key.
      std::string const _( "enum" );
      const nlohmann::json dummy{ { _, phase } };
      try
      {
        tmp.emplace( dummy[_], dataProvider( phase ) );
      }
      catch( const std::out_of_range & e )
      {
        // Intentionally left blank: the phase may not exist and
        // pvt::MultiphaseSystemProperties currently does not provide its phases.
      }
    }

    return tmp;
  };

  j[Keys::MASS_DENSITY] = f( [&msp]( PHASE_TYPE phase ) { return msp.getMassDensity( phase ); } );
  j[Keys::MOLE_DENSITY] = f( [&msp]( PHASE_TYPE phase ) { return msp.getMoleDensity( phase ); } );
  j[Keys::MOLE_COMPOSITION] = f( [&msp]( PHASE_TYPE phase ) { return msp.getMoleComposition( phase ); } );
  j[Keys::MOLECULAR_WEIGHT] = f( [&msp]( PHASE_TYPE phase ) { return msp.getMolecularWeight( phase ); } );
  j[Keys::VISCOSITY] = f( [&msp]( PHASE_TYPE phase ) { return msp.getViscosity( phase ); } );
  j[Keys::PHASE_MOLE_FRACTION] = f( [&msp]( PHASE_TYPE phase ) { return msp.getPhaseMoleFraction( phase ); } );
}

}
