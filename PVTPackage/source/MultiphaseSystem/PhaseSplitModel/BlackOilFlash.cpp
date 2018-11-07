#include "BlackOilFlash.hpp"
#include <algorithm>

namespace PVTPackage
{
	BlackOilFlash::BlackOilFlash(): m_OilModel(nullptr), m_GasModel(nullptr), m_WaterModel(nullptr)
	{
		
	}

  void BlackOilFlash::set_PhaseState(MultiphaseSystemProperties & out_variables)
  {

  }

  bool BlackOilFlash::ComputeEquilibrium(MultiphaseSystemProperties & out_variables)
  {
    return false;
  }

}


