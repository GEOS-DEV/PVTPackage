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
#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "PhaseSplitModel/BlackOilFlash.hpp"

namespace PVTPackage
{

class BlackOilMultiphaseSystem final : public MultiphaseSystem
{
public:

  BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                           std::vector<std::vector<double>> PVTO,
                           std::vector<std::vector<double>> PVTG,
                           std::vector<double> PVTW,
                           std::vector<double> surface_densities,
                           std::vector<double> molar_weights);

  BlackOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                           std::vector<std::string> table_file_names,
                           std::vector<double> surface_densities,
                           std::vector<double> molar_weights);

  ~BlackOilMultiphaseSystem() override;


protected:

  void CreatePhases(std::vector<PHASE_TYPE> phase_types,
                    std::vector<std::vector<std::vector<double>>> phase_tables,
                    std::vector<double> surface_densities,
                    std::vector<double> molar_weights);

  void ReadTable(const std::string & filename, std::vector<std::vector<double>> & data, unsigned int min_row_len);

};


}

