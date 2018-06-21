#pragma once
#include <vector>
#include "PVTOdata.hpp"

namespace PVTPackage
{
	namespace BlackOilUtils
	{
		void RefineTable(std::vector<std::vector<std::vector<double>>>& table, size_t nlevel);
	}

}
