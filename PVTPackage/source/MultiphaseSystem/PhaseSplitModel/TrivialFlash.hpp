#pragma once
#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"

namespace PVTPackage
{

	class TrivialFlash final : public CompositionalFlash
	{
	public:

		TrivialFlash(std::unordered_map<PHASE_TYPE, PhaseModel*>& phase_models)
			: CompositionalFlash(phase_models)
		{
		}

		~TrivialFlash() override = default;

		void ComputeEquilibrium(double pressure, double temperature, std::vector<double> feed, MultiphaseSystemProperties* out_variables) override;

	protected:


	};

}


