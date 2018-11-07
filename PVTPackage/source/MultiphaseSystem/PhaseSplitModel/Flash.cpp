#include "MultiphaseSystem/PhaseSplitModel/Flash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "Utils/math.hpp"
#include <algorithm>

namespace PVTPackage
{


	bool Flash::ComputeEquilibriumAndDerivatives(MultiphaseSystemProperties& out_variables)
	{
		//Compute Equilibrium
		bool success = ComputeEquilibrium(out_variables);

		//Finite difference derivatives
		success &= ComputeFiniteDifferenceDerivatives(out_variables);

		return success;
	}

	bool Flash::ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables)
	{
		const auto& pressure = out_variables.Pressure;
		const auto& temperature = out_variables.Temperature;
		const auto& feed = out_variables.Feed;

		auto  sqrtprecision = sqrt(std::numeric_limits<double>::epsilon());
		double epsilon = 0;

		MultiphaseSystemProperties props_eps = out_variables;

		bool success = true;

		////Pressure
		epsilon = sqrtprecision * (std::fabs(pressure) + sqrtprecision);
		props_eps.Pressure = pressure + epsilon;
		success &= ComputeEquilibrium(props_eps);
		props_eps.Pressure = pressure;
		out_variables.UpdateDerivative_dP_FiniteDifference(props_eps, epsilon);

		////Temperature
		epsilon = sqrtprecision * (std::fabs(temperature) + sqrtprecision);
		props_eps.Temperature = temperature + epsilon;
		success &= ComputeEquilibrium(props_eps);
		props_eps.Temperature = temperature;
		out_variables.UpdateDerivative_dT_FiniteDifference(props_eps, epsilon);

		//Feed
		for (size_t i = 0; i < feed.size(); ++i)
		{
			epsilon = sqrtprecision * (std::fabs(feed[i]) + sqrtprecision);
			if (feed[i] + epsilon > 1)
			{
				epsilon = -epsilon;
			}
			auto save_feed = feed;
			props_eps.Feed[i] = feed[i] + epsilon;
			props_eps.Feed = math::Normalize(props_eps.Feed);
			success &= ComputeEquilibrium(props_eps);
			props_eps.Feed = save_feed;
			out_variables.UpdateDerivative_dz_FiniteDifference(i, props_eps, epsilon);
		}

		return success;
	}

}


