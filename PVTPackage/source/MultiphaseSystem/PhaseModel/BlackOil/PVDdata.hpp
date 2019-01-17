#pragma once

#include <vector>

namespace PVTPackage
{

struct PVDdata
{
  friend class DeadOil_GasModel;

  //--Data
  //
  std::vector<double> Pressure;
  size_t NPoints;
  std::vector<double> B;
  std::vector<double> Viscosity;


};

}
