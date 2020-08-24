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

#include "BlackOilMultiphaseSystem.hpp"

#include "PhaseModel/BlackOil/BlackOil_GasModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "PhaseModel/BlackOil/BlackOil_WaterModel.hpp"
#include "Utils/FileUtils.hpp"
#include <unordered_map>
#include <utility>

namespace PVTPackage
{

BlackOilMultiphaseSystem::BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                                                   std::vector<std::vector<double>> PVTO,
                                                   std::vector<std::vector<double>> PVTG,
                                                   std::vector<double> PVTW,
                                                   std::vector<double> surface_densities,
                                                   std::vector<double> molar_weights)
  : MultiphaseSystem(phase_types.size(), phase_types)
{
  CreatePhases(phase_types,
               { std::move(PVTO), std::move(PVTG), { std::move(PVTW) } },
               std::move(surface_densities),
               std::move(molar_weights));

  //Create Flash pointer
  m_Flash = new BlackOilFlash();
}

BlackOilMultiphaseSystem::BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                                                   std::vector<std::string> table_file_names,
                                                   std::vector<double> surface_densities,
                                                   std::vector<double> molar_weights)
  : MultiphaseSystem(phase_types.size(), phase_types)
{
  auto num_phase = static_cast<unsigned>(phase_types.size());
  std::vector<std::vector<std::vector<double>>> tables(num_phase);

  for (unsigned i = 0; i < num_phase; ++i)
  {
    const int row_min_len = (phase_types[i] == PHASE_TYPE::LIQUID_WATER_RICH) ? 4 : 3;
    ReadTable(table_file_names[i], tables[i], row_min_len);
  }

  CreatePhases(phase_types, tables, std::move(surface_densities), std::move(molar_weights));

  //Create Flash pointer
  m_Flash = new BlackOilFlash();
}

void BlackOilMultiphaseSystem::CreatePhases(std::vector<PHASE_TYPE> phase_types,
                                            std::vector<std::vector<std::vector<double>>> phase_tables,
                                            std::vector<double> surface_densities,
                                            std::vector<double> molar_weights)
{
  //Create Phase Models
  for (size_t i = 0; i != phase_types.size(); ++i)
  {
    auto & phase_ptr = m_MultiphaseProperties.PhaseModels[phase_types[i]];
    switch (phase_types[i])
    {
      case PHASE_TYPE::OIL:
        phase_ptr = std::make_shared<BlackOil_OilModel>(phase_tables[i], surface_densities[i], molar_weights[i]);
        break;
      case PHASE_TYPE::GAS:
        phase_ptr = std::make_shared<BlackOil_GasModel>(phase_tables[i], surface_densities[i], molar_weights[i]);
        break;
      case PHASE_TYPE::LIQUID_WATER_RICH:
        ASSERT(phase_tables[i].size() == 1, "Too many lines in water properties table");
        phase_ptr = std::make_shared<BlackOil_WaterModel>(phase_tables[i][0], surface_densities[i], molar_weights[i]);
        break;
      default:
        LOGERROR("Phase type not supported for Black Oil model");
    }
  }

  //Check consistency between PVTO and PVTG
  //TODO

  //Check if both oil and gas are defined
  ASSERT((m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::OIL) != m_MultiphaseProperties.PhaseModels.end())
         && (m_MultiphaseProperties.PhaseModels.find(PHASE_TYPE::GAS) != m_MultiphaseProperties.PhaseModels.end()),
         "Both oil and gas phase must be defined for BO model");
}


BlackOilMultiphaseSystem::~BlackOilMultiphaseSystem()
{
  delete m_Flash;
}

void BlackOilMultiphaseSystem::ReadTable(const std::string & filename, std::vector<std::vector<double>> & data,
                                         unsigned int min_row_len)
{
  FileUtils::ReadTable(filename, data);

  for (auto & row : data)
  {
    ASSERT(row.size() >= min_row_len, "Too few entries in a row of table " + filename
                                      + ", minimum " + std::to_string(min_row_len) + " required");
  }
}


}
