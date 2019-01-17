#pragma once
#include "MultiphaseSystem/MultiphaseSystem.hpp"
#include "PhaseSplitModel/BlackOilFlash.hpp"

namespace PVTPackage
{

class DeadOilMultiphaseSystem final : public MultiphaseSystem
{
public:

	DeadOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                           std::vector<std::vector<double>> PVDO,
                           std::vector<std::vector<double>> PVDG,
                           std::vector<double> PVTW,
                           std::vector<double> surface_densities,
                           std::vector<double> molar_weights);

	DeadOilMultiphaseSystem(std::vector<PHASE_TYPE> phase_types,
                           std::vector<std::string> table_file_names,
                           std::vector<double> surface_densities,
                           std::vector<double> molar_weights);

  ~DeadOilMultiphaseSystem() override;


protected:

  void CreatePhases(std::vector<PHASE_TYPE> phase_types,
                    std::vector<std::vector<std::vector<double>>> phase_tables,
                    std::vector<double> surface_densities,
                    std::vector<double> molar_weights);

  void ReadTable(const std::string & filename, std::vector<std::vector<double>> & data, unsigned int min_row_len);

};


}

