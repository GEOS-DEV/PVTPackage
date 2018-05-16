#pragma once
#include <vector>

namespace PVTPackage
{
	struct PhaseProperties;

	class PhaseModel
	{
	public:

		PhaseModel()
		{
		}

		virtual void ComputeAllProperties (double Pressure, double Temperature, std::vector<double>& composition, PhaseProperties* props_out)=0;

	};
}
