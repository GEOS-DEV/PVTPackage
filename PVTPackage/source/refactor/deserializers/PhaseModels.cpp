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

#include "refactor/deserializers/PhaseModels.hpp"

#include "refactor/JsonKeys.hpp"

#include "refactor/passiveDataStructures/PhaseModel.hpp"

#include "refactor/deserializers/PVTEnums.hpp"
#include "refactor/deserializers/ComponentProperties.hpp"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace PVTPackage {
namespace refactor {
namespace pds {

void from_json( const nlohmann::json & j,
                pds::CubicEoSPhaseModel & model )
{
  j.at( CubeEoSPhaseModelsKeys::COMPONENT_PROPERTIES ).get_to( model.m_ComponentsProperties );
  j.at( CubeEoSPhaseModelsKeys::EOS ).get_to( model.m_EOSType );
  j.at( CubeEoSPhaseModelsKeys::PHASE_TYPE_ ).get_to( model.m_PhaseType );
}

void from_json( const nlohmann::json & j,
                pds::PVTGdata & data )
{
  j.at( PVTGDataKeys::RV ).get_to( data.Rv );
  j.at( PVTGDataKeys::DEW_PRESSURE ).get_to( data.DewPressure );

  j.at( PVTGDataKeys::N_SATURATED_POINTS ).get_to( data.NSaturatedPoints );
  j.at( PVTGDataKeys::SATURATED_BG ).get_to( data.SaturatedBg );
  j.at( PVTGDataKeys::SATURATED_VISCOSITY ).get_to( data.SaturatedViscosity );

  j.at( PVTGDataKeys::UNDERSATURATED_RV ).get_to( data.UndersaturatedRv );
  j.at( PVTGDataKeys::UNDERSATURATED_BG ).get_to( data.UndersaturatedBg );
  j.at( PVTGDataKeys::UNDERSATURATED_VISCOSITY ).get_to( data.UndersaturatedViscosity );

  j.at( PVTGDataKeys::MAX_RELATIVE_RV ).get_to( data.MaxRelativeRv );
  j.at( PVTGDataKeys::MIN_RELATIVE_RV ).get_to( data.MinRelativeRv );
}

void from_json( const nlohmann::json & j,
                pds::BlackOilGasModel & model )
{
  j.at( BlackOilGasModelKeys::PVTG_DATA ).get_to( model.m_PVTG );

  j.at( BlackOilGasModelKeys::MIN_PRESSURE ).get_to( model.min_Pressure );
  j.at( BlackOilGasModelKeys::MAX_PRESSURE ).get_to( model.max_Pressure );

  j.at( BlackOilGasModelKeys::SURFACE_MASS_DENSITY ).get_to( model.m_SurfaceMassDensity );
  j.at( BlackOilGasModelKeys::SURFACE_MOLE_DENSITY ).get_to( model.m_SurfaceMoleDensity );
  j.at( BlackOilGasModelKeys::SURFACE_MOLECULAR_WEIGHT ).get_to( model.m_SurfaceMolecularWeight );
}

void from_json( const nlohmann::json & j,
                pds::PVTOdata & data )
{
  j.at( PVTODataKeys::RS ).get_to( data.Rs );
  j.at( PVTODataKeys::BUBBLE_PRESSURE ).get_to( data.BubblePressure );

  j.at( PVTODataKeys::N_SATURATED_POINTS ).get_to( data.NSaturatedPoints );
  j.at( PVTODataKeys::SATURATED_BO ).get_to( data.SaturatedBo );
  j.at( PVTODataKeys::SATURATED_VISCOSITY ).get_to( data.SaturatedViscosity );

  j.at( PVTODataKeys::UNDERSATURATED_PRESSURE ).get_to( data.UndersaturatedPressure );
  j.at( PVTODataKeys::UNDERSATURATED_BO ).get_to( data.UndersaturatedBo );
  j.at( PVTODataKeys::UNDERSATURATED_VISCOSITY ).get_to( data.UndersaturatedViscosity );

  j.at( PVTODataKeys::MAX_RELATIVE_PRESSURE ).get_to( data.MaxRelativePressure );
  j.at( PVTODataKeys::MIN_RELATIVE_PRESSURE ).get_to( data.MinRelativePressure );
}

void from_json( const nlohmann::json & j,
                pds::BlackOilOilModel & model )
{
  j.at( BlackOilOilModelKeys::PVTO_DATA ).get_to( model.m_PVTO );

  j.at( BlackOilOilModelKeys::MIN_PRESSURE ).get_to( model.min_Pressure );
  j.at( BlackOilOilModelKeys::MAX_PRESSURE ).get_to( model.max_Pressure );

  j.at( BlackOilOilModelKeys::SURFACE_MASS_DENSITY ).get_to( model.m_SurfaceMassDensity );
  j.at( BlackOilOilModelKeys::SURFACE_MOLE_DENSITY ).get_to( model.m_SurfaceMoleDensity );
  j.at( BlackOilOilModelKeys::SURFACE_MOLECULAR_WEIGHT ).get_to( model.m_SurfaceMolecularWeight );
}

void from_json( const nlohmann::json & j,
                pds::PVTWdata & data )
{
  j.at( PVTWDataKeys::REFERENCE_PRESSURE ).get_to( data.ReferencePressure );
  j.at( PVTWDataKeys::BW ).get_to( data.Bw );
  j.at( PVTWDataKeys::COMPRESSIBILITY ).get_to( data.Compressibility );
  j.at( PVTWDataKeys::VISCOSITY ).get_to( data.Viscosity );
}

void from_json( const nlohmann::json & j,
                pds::BlackOilWaterModel & model )
{
  j.at( BlackOilWaterModelKeys::PVTW_DATA ).get_to( model.m_PVTW );
  j.at( BlackOilWaterModelKeys::SURFACE_MASS_DENSITY ).get_to( model.m_SurfaceMassDensity );
  j.at( BlackOilWaterModelKeys::SURFACE_MOLE_DENSITY ).get_to( model.m_SurfaceMoleDensity );
  j.at( BlackOilWaterModelKeys::SURFACE_MOLECULAR_WEIGHT ).get_to( model.m_SurfaceMolecularWeight );
}

void from_json( const nlohmann::json & j,
                pds::PVDdata & data )
{
  j.at( PVDDataKeys::PRESSURE ).get_to( data.Pressure );
  j.at( PVDDataKeys::N_POINTS ).get_to( data.NPoints );
  j.at( PVDDataKeys::B ).get_to( data.B );
  j.at( PVDDataKeys::VISCOSITY ).get_to( data.Viscosity );
}

void from_json( const nlohmann::json & j,
                pds::DeadOilPhaseModel & model )
{
  j.at( DeadOilModelKeys::PHASE_TYPE_ ).get_to( model.m_type );
  j.at( DeadOilModelKeys::PVD_DATA ).get_to( model.m_PVD );

  j.at( DeadOilModelKeys::MIN_PRESSURE ).get_to( model.min_Pressure );
  j.at( DeadOilModelKeys::MAX_PRESSURE ).get_to( model.max_Pressure );

  j.at( DeadOilModelKeys::SURFACE_MASS_DENSITY ).get_to( model.m_SurfaceMassDensity );
  j.at( DeadOilModelKeys::SURFACE_MOLE_DENSITY ).get_to( model.m_SurfaceMoleDensity );
  j.at( DeadOilModelKeys::SURFACE_MOLECULAR_WEIGHT ).get_to( model.m_SurfaceMolecularWeight );
}

} // end of namespace pds
} // end of namespace refactor
} // end of namespace PVTPackage
