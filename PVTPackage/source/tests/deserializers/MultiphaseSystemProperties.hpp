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
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.	
 * ------------------------------------------------------------------------------------------------------------	
 */

#ifndef PVTPACKAGE_DESERIALIZERS_MULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_DESERIALIZERS_MULTIPHASESYSTEMPROPERTIES_HPP

#include "./PropertyAndDerivatives.hpp"
#include "./PVTEnums.hpp"

#include "../passiveDataStructures/MultiphaseSystemProperties.hpp"
#include "../passiveDataStructures/PropertyAndDerivatives.hpp"
#include "../passiveDataStructures/PVTEnums.hpp"

#include <nlohmann/json.hpp>

namespace PVTPackage
{
namespace tests
{
namespace pds
{

void from_json( const nlohmann::json & j,
                PDSMSP & props )
{
  // FIXME Use a typedef
  const std::map< std::string, std::map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double > > & > mScal{
    { PublicAPIKeys::MSP::MASS_DENSITY,        props.m_massDensity },
    { PublicAPIKeys::MSP::MOLE_DENSITY,        props.m_moleDensity },
    { PublicAPIKeys::MSP::MOLECULAR_WEIGHT,    props.m_molecularWeight },
    { PublicAPIKeys::MSP::PHASE_MOLE_FRACTION, props.m_phaseMoleFraction },
    { PublicAPIKeys::MSP::VISCOSITY,           props.m_viscosity }
  };

  // FIXME Use a typedef
  const std::map< std::string, std::map< pds::PHASE_TYPE, pds::VectorPropertyAndDerivatives< double > > & > mVec{
    { PublicAPIKeys::MSP::MOLE_COMPOSITION, props.m_moleComposition }
  };

  auto f = [&j]( const auto & mapping )
  {
    for( const auto & kv: mapping )
    {
      // kv.first is the json key.
      // kv.second is the reference to the data map to be filled.
      for( const auto & elt: j.at( kv.first ).items() )
      {
        // Small trick to convert the input string (elt.key()) to it's actual enum type.
        const nlohmann::json tmp = elt.key();
        const pds::PHASE_TYPE phase = tmp.get< pds::PHASE_TYPE >();
        // Now we can store where indicated.
        elt.value().get_to( kv.second[phase] );
      }
    }
  };

  f( mScal );
  f( mVec );
}

}
}
}

#endif //PVTPACKAGE_DESERIALIZERS_MULTIPHASESYSTEMPROPERTIES_HPP
