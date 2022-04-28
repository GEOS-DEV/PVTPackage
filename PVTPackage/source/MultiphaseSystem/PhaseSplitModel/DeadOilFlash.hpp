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

#include "MultiphaseSystem/MultiphaseSystemProperties/DeadOilFlashMultiphaseSystemProperties.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/DeadOil_PhaseModel.hpp"

namespace PVTPackage
{

class DeadOilFlash
{
public:

  /**
   * @brief Constructor for the three-phase Dead-Oil model
   */  
  DeadOilFlash( std::vector< std::vector< double > > const & PVDO,
                double oilSurfaceMassDensity,
                double oilSurfaceMolecularWeight,
                std::vector< std::vector< double > > const & PVDG,
                double gasSurfaceMassDensity,
                double gasSurfaceMolecularWeight,
                std::vector< double > const & PVTW,
                double waterSurfaceMassDensity,
                double waterSurfaceMolecularWeight );

  /**
   * @brief Constructor for the two-phase oil-gas Dead-Oil model
   */  
  DeadOilFlash( std::vector< std::vector< double > > const & PVDO,
                double oilSurfaceMassDensity,
                double oilSurfaceMolecularWeight,
                std::vector< std::vector< double > > const & PVDG,
                double gasSurfaceMassDensity,
                double gasSurfaceMolecularWeight );

  /**
   * @brief Constructor for the two-phase oil-water Dead-Oil model
   */  
  DeadOilFlash( std::vector< std::vector< double > > const & PVDO,
                double oilSurfaceMassDensity,
                double oilSurfaceMolecularWeight,
                std::vector< double > const & PVTW,
                double waterSurfaceMassDensity,
                double waterSurfaceMolecularWeight );
  
  DeadOil_PhaseModel const & getOilPhaseModel() const;

  /**
   * @brief Reference to a gas model
   * @warning The result returned by this function will be void for a two-phase oil-water model
   */
  DeadOil_PhaseModel const & getGasPhaseModel() const;

  /**
   * @brief Reference to a water model
   * @warning The result returned by this function will be void for a two-phase oil-gas model
   */
  BlackOil_WaterModel const & getWaterPhaseModel() const;

  bool computeEquilibrium( DeadOilFlashMultiphaseSystemProperties & sysProps ) const;

private:

  DeadOil_PhaseModel m_oilPhaseModel;
  std::unique_ptr< DeadOil_PhaseModel > m_gasPhaseModel;
  std::unique_ptr< BlackOil_WaterModel > m_waterPhaseModel;
};

}
