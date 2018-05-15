#pragma once
#include "Utils/Assert.hpp"
#include  <vector>

struct ComponentProperties
{
	ComponentProperties(size_t n_components, std::vector<std::string> labels, std::vector<double> mw, std::vector<double> tc,
		std::vector<double> pc, std::vector<double> omega)
		: NComponents(n_components),
		  Label(labels),
		  Mw(mw),
		  Pc(pc),
		  Tc(tc),
		  Omega(omega),
		  BIC(std::vector<std::vector<double>>(NComponents, std::vector<double>(NComponents, 0))),
		  VolumeShift(std::vector<std::vector<double>>(NComponents, std::vector<double>(2, 0))), WaterIndex(-1)
	{
		ASSERT((n_components == labels.size()) && (n_components == mw.size()) && (n_components == tc.size()) && (n_components
			== pc.size()) && (n_components == omega.size()), "Dimension Mismatch.");
		for (size_t i = 0; i != NComponents; ++i)
		{
			if (Label[i]=="Water" || Label[i]=="water" || Label[i]=="H2O" || Label[i]=="h2o")
				WaterIndex = i;
		}
	}

	const size_t NComponents;
	const std::vector<std::string> Label;
	const std::vector<double> Mw;
	const std::vector<double> Pc;
	const std::vector<double> Tc;
	const std::vector<double> Omega;
	const std::vector<std::vector<double>> BIC;
	const std::vector<std::vector<double>> VolumeShift;
	size_t WaterIndex;
};
