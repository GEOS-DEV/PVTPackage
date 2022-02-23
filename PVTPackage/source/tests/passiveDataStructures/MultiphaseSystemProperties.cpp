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

#include "./MultiphaseSystemProperties.hpp"

namespace PVTPackage
{
namespace tests
{
namespace pds
{

pds::ScalarPropertyAndDerivatives< double > const & PDSMSP::getMassDensity( pds::PHASE_TYPE const & phase ) const
{
  return m_massDensity.at( phase );
}

pds::VectorPropertyAndDerivatives< double > const & PDSMSP::getMoleComposition( pds::PHASE_TYPE const & phase ) const
{
  return m_moleComposition.at( phase );
}

pds::ScalarPropertyAndDerivatives< double > const & PDSMSP::getMoleDensity( pds::PHASE_TYPE const & phase ) const
{
  return m_moleDensity.at( phase );
}

pds::ScalarPropertyAndDerivatives< double > const & PDSMSP::getMolecularWeight( pds::PHASE_TYPE const & phase ) const
{
  return m_molecularWeight.at( phase );
}

pds::ScalarPropertyAndDerivatives< double > const & PDSMSP::getPhaseMoleFraction( pds::PHASE_TYPE const & phase ) const
{
  return m_phaseMoleFraction.at( phase );
}

pds::ScalarPropertyAndDerivatives< double > const & PDSMSP::getViscosity( pds::PHASE_TYPE const & phase ) const
{
  return m_viscosity.at( phase );
}

std::set< pds::PHASE_TYPE > PDSMSP::getPhases() const
{
  std::set< pds::PHASE_TYPE > result;

  auto addPhase = [&result]( const auto & m )
  {
    for( const auto & kv: m )
    {
      result.insert( kv.first );
    }
  };

  for( const auto & m: { m_massDensity, m_moleDensity, m_molecularWeight, m_phaseMoleFraction, m_viscosity } )
  {
    addPhase( m );
  }
  addPhase( m_moleComposition );

  return result;
}

}
}
}
