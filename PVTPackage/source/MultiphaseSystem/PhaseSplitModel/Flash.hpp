#pragma once

#include "MultiphaseSystem/PVTEnums.hpp"

namespace PVTPackage
{

struct MultiphaseSystemProperties;

class Flash
{
public:

  virtual ~Flash() = default;

  Flash() = default;

  bool ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables);
  virtual bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) = 0;
  virtual void set_PhaseState(MultiphaseSystemProperties& out_variables) = 0;

protected:

  bool ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables);
};

}
