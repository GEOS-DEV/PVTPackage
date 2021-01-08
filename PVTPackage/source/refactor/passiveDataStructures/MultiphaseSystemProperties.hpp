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

#ifndef PVTPACKAGE_PDS_MULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_PDS_MULTIPHASESYSTEMPROPERTIES_HPP

#include "refactor/passiveDataStructures/PropertyAndDerivatives.hpp"
#include "refactor/passiveDataStructures/PVTEnums.hpp"

#include <map>
#include <set>

namespace PVTPackage
{
namespace refactor
{
namespace pds
{

class PDSMSP // stands for pds::MultiphaseSystemProperties
{
public:

  std::map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double > > m_massDensity;
  std::map< pds::PHASE_TYPE, pds::VectorPropertyAndDerivatives< double > > m_moleComposition;
  std::map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double > > m_moleDensity;
  std::map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double > > m_molecularWeight;
  std::map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double > > m_phaseMoleFraction;
  std::map< pds::PHASE_TYPE, pds::ScalarPropertyAndDerivatives< double > > m_viscosity;

  pds::ScalarPropertyAndDerivatives< double > const & getMassDensity( pds::PHASE_TYPE const & phase ) const;
  pds::VectorPropertyAndDerivatives< double > const & getMoleComposition( pds::PHASE_TYPE const & phase ) const;
  pds::ScalarPropertyAndDerivatives< double > const & getMoleDensity( pds::PHASE_TYPE const & phase ) const;
  pds::ScalarPropertyAndDerivatives< double > const & getMolecularWeight( pds::PHASE_TYPE const & phase ) const;
  pds::ScalarPropertyAndDerivatives< double > const & getPhaseMoleFraction( pds::PHASE_TYPE const & phase ) const;
  pds::ScalarPropertyAndDerivatives< double > const & getViscosity( pds::PHASE_TYPE const & phase ) const;

  std::set< pds::PHASE_TYPE > getPhases() const;
};

}
}
}

#endif //PVTPACKAGE_PDS_MULTIPHASESYSTEMPROPERTIES_HPP
