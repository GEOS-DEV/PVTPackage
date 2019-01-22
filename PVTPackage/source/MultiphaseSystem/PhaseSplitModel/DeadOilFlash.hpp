#pragma once

#include "Flash.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <unordered_map>
#include <list>
#include "Flash.hpp"

namespace PVTPackage
{

struct MultiphaseSystemProperties;
class PhaseModel;

class DeadOilFlash : public Flash
{
public:

  DeadOilFlash() = default;
  bool ComputeEquilibrium(MultiphaseSystemProperties& out_variables) override;
  void set_PhaseState(MultiphaseSystemProperties& out_variables) override;

  ~DeadOilFlash() override = default;


};

}
