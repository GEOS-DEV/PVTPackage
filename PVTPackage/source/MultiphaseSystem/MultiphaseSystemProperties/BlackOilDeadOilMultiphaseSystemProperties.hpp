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

#ifndef PVTPACKAGE_BLACKOILDEADOILMULTIPHASESYSTEMPROPERTIES_HPP
#define PVTPACKAGE_BLACKOILDEADOILMULTIPHASESYSTEMPROPERTIES_HPP

#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOilDeadOilProperties.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties/FactorMultiphaseSystemProperties.hpp"

#include "pvt/pvt.hpp"

namespace PVTPackage
{

class BlackOilDeadOilMultiphaseSystemProperties : public FactorMultiphaseSystemProperties
{
public:

  explicit BlackOilDeadOilMultiphaseSystemProperties( std::vector< pvt::PHASE_TYPE > const & phases );

  void setOilModelProperties( BlackOilDeadOilProperties const & props );

  void setGasModelProperties( BlackOilDeadOilProperties const & props );

  void setWaterModelProperties( BlackOilDeadOilProperties const & props );

protected:
  
  /**
   * @brief Returns the number of components (here, it is equal to the number of phases) 
   */  
  std::size_t getNComponents() const;
  
private:

  /**
   * @brief Sets the properties computed by the model for given @p phase.
   * @param phase The phase for which we want to set properties.
   * @param properties The new values.
   *
   * Mainly defines mass density, mole density, viscosity, and molecular weight.
   */
  void setModelProperties( pvt::PHASE_TYPE const & phase,
                           BlackOilDeadOilProperties const & props );

};

}

#endif //PVTPACKAGE_BLACKOILDEADOILMULTIPHASESYSTEMPROPERTIES_HPP
