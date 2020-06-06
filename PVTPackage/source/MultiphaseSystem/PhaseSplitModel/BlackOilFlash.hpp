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

#pragma once

#include "MultiphaseSystem/MultiphaseSystemProperties/BlackOilFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"


namespace PVTPackage
{

class BlackOilFlash
{
public:

  BlackOilFlash( const std::vector< std::vector< double > > & PVTO,
                 double oilSurfaceMassDensity,
                 double oilSurfaceMolecularWeight,
                 const std::vector< std::vector< double > > & PVTG,
                 double gasSurfaceMassDensity,
                 double gasSurfaceMolecularWeight,
                 const std::vector< double > & PVTW,
                 double waterSurfaceMassDensity,
                 double waterSurfaceMolecularWeight );


  bool computeEquilibrium( BlackOilFlashMultiphaseSystemProperties & sysProperties ) const;

private:

  BlackOil_OilModel m_oilPhaseModel;
  BlackOil_GasModel m_gasPhaseModel;
  BlackOil_WaterModel m_waterPhaseModel;

};

}
