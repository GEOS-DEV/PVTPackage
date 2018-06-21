#include "BlackOilFlash.hpp"
#include <algorithm>

namespace PVTPackage
{
	BlackOilFlash::BlackOilFlash(std::list<PHASE_TYPE> phase_types): m_OilModel(nullptr), m_GasModel(nullptr),
	                                                                 m_WaterModel(nullptr)
	{
		
	}
}


