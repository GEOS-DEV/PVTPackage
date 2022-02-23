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

#include "BlackOilDeadOilMultiphaseSystemProperties.hpp"

#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOilDeadOilProperties.hpp"

#include "pvt/pvt.hpp"

#include <map>

namespace PVTPackage
{

BlackOilDeadOilMultiphaseSystemProperties::BlackOilDeadOilMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases )
  :
  FactorMultiphaseSystemProperties( phases, phases.size() ) // for both Black-Oil and Dead-Oil, the number of components is equal to the number of phases, hence we use phases.size() to fill nComponents in FactorMultiphaseSystemProperties
{

}

void BlackOilDeadOilMultiphaseSystemProperties::setModelProperties( pvt::PHASE_TYPE const & phase,
                                                                    BlackOilDeadOilProperties const & props )
{
  m_massDensity.at( phase ).value = props.massDensity;
  m_moleDensity.at( phase ).value = props.moleDensity;
  m_viscosity.at( phase ).value = props.viscosity;
  m_molecularWeight.at( phase ).value = props.moleDensity > 0 ? props.massDensity / props.moleDensity : 0.0;
}

void BlackOilDeadOilMultiphaseSystemProperties::setOilModelProperties( BlackOilDeadOilProperties const & props )
{
  setModelProperties( pvt::PHASE_TYPE::OIL, props );
}

void BlackOilDeadOilMultiphaseSystemProperties::setGasModelProperties( BlackOilDeadOilProperties const & props )
{
  setModelProperties( pvt::PHASE_TYPE::GAS, props );
}

void BlackOilDeadOilMultiphaseSystemProperties::setWaterModelProperties( BlackOilDeadOilProperties const & props )
{
  setModelProperties( pvt::PHASE_TYPE::LIQUID_WATER_RICH, props );
}

std::size_t BlackOilDeadOilMultiphaseSystemProperties::getNComponents() const
{
  // for Black-Oil and Dead-Oil the number of components is equal to the number of phases
  return getPhases().size(); 
}
  
}
