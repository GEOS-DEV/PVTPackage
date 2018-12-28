#pragma once
#include <vector>
#include "PVTOdata.hpp"

namespace PVTPackage
{
	namespace BlackOilUtils
	{
		void RefineTable(std::vector<std::vector<std::vector<double>>>& table, std::size_t nlevel);
	}

}
