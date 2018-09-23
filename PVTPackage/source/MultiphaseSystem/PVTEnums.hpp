#pragma once
#include <unordered_map>
#include <vector>

namespace PVTPackage
{
	enum class PVT_TYPE { BLACK_OIL = 0, DEAD = 0, GENERAL = 1, MODIFIED = 2, API_TRACKING = 3, COMPOSITIONAL = 4, UNKNOWN = -1};
	enum class COMPOSITIONAL_FLASH_TYPE { TRIVIAL = 0, WILSON_KVALUES = 1, TABULATED_KVALUES = 2, FREE_WATER_FLASH = 3, THREE_PHASE_FLASH = 4, UNKNOWN = -1 };
	enum class EOS_TYPE { REDLICH_KWONG_SOAVE = 1, PENG_ROBINSON = 0, UNKNOWN = -1 };
	enum class PHASE_TYPE { LIQUID_WATER_RICH = 2, OIL = 1, GAS = 0, UNKNOWN = -1 };
	enum class PHASE_STATE { GAS = 6, OIL = 5, WATER = 4, OIL_GAS = 3, GAS_WATER = 2, OIL_WATER = 1, OIL_GAS_WATER = 0, UNKNOWN = -1 };

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif

	//Phase State
	const std::unordered_map<std::vector<bool>, PHASE_STATE> PhaseStateMap =
	{
		{ { 1,1,1 } ,PHASE_STATE::OIL_GAS_WATER },
		{ { 1,0,1 } ,PHASE_STATE::OIL_WATER },
		{ { 1,1,0 } ,PHASE_STATE::OIL_GAS },
		{ { 0,1,1 } ,PHASE_STATE::GAS_WATER },
		{ { 0,0,1 } ,PHASE_STATE::WATER },
		{ { 1,0,0 } ,PHASE_STATE::OIL },
		{ { 0,1,0 } ,PHASE_STATE::GAS },
	};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

}
