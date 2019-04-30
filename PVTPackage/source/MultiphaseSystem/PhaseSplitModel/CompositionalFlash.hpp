#pragma once

#include <list>
#include "MultiphaseSystem/PVTEnums.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include "Flash.hpp"

namespace PVTPackage
{

struct MultiphaseSystemProperties;

class CompositionalFlash : public Flash
{
public:

  ~CompositionalFlash() override = default;

  explicit CompositionalFlash(const ComponentProperties& component_properties)
    : m_ComponentsProperties(component_properties) {}

  double SolveRachfordRiceEquation(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index);
  double RachfordRiceFunction(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, double x);
  double dRachfordRiceFunction_dx(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, double x);



protected:

  ComponentProperties m_ComponentsProperties;

  //Wilson K-values
  std::vector<double> ComputeWilsonGasLiquidKvalue(double Pressure, double Temperature) const;
  std::vector<double> ComputeWaterGasKvalue(double Pressure, double Temperature) const;
  std::vector<double> ComputeWaterOilKvalue(double Pressure, double Temperature) const;
};

}
