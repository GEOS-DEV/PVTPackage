#include "BlackOilFlash.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include <algorithm>
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_OilModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_GasModel.hpp"

namespace PVTPackage
{
	BlackOilFlash::BlackOilFlash(): m_OilModel(nullptr), m_GasModel(nullptr),
	                                                                 m_WaterModel(nullptr)
	{
		
	}

	void BlackOilFlash::set_PhaseState(MultiphaseSystemProperties& out_variables)
	{

		out_variables.PhaseState = PhaseStateMap.at
		({ out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value > 0.,
			out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value > 0. ,
			out_variables.PhaseMoleFraction.at(PHASE_TYPE::LIQUID_WATER_RICH).value > 0.
			});
	}



	bool BlackOilFlash::ComputeEquilibrium(MultiphaseSystemProperties& out_variables)
	{

		const auto& pressure = out_variables.Pressure;
		const auto& temperature = out_variables.Temperature;
		// Component index are 0=OIL, 1=GAS, 2=WATER
		auto& zo = out_variables.Feed[0];
		auto& zg = out_variables.Feed[1];

		const auto& oil_phase_model = static_cast<BlackOil_OilModel*>(out_variables.PhaseModels.at(PHASE_TYPE::OIL));
		const auto& gas_phase_model = static_cast<BlackOil_GasModel*>(out_variables.PhaseModels.at(PHASE_TYPE::GAS));

		

		
	

		//Oil
		 const auto& oil_surface_mole_density =  oil_phase_model->GetSurfaceOilMoleDensity();
		 auto Rs = oil_phase_model->ComputeRs(pressure);

		//Gas
		 const auto& gas_surface_mole_density = gas_phase_model->GetSurfaceGasMoleDensity();
		 auto Rv = gas_phase_model->ComputeRv(pressure);
		 


		 ////Phase state
		 //auto oil_undersaturated = ((zg / gas_surface_mole_density - Rs * zo / oil_surface_mole_density) < 0);
		 //auto gas_undersaturated = ((zo / oil_surface_mole_density - Rv * zg / gas_surface_mole_density) < 0);

		 //Phase compositions
		 auto Ko = Rv * (oil_surface_mole_density + gas_surface_mole_density * Rs) / (gas_surface_mole_density + oil_surface_mole_density * Rv);
		 auto Kg = (oil_surface_mole_density + gas_surface_mole_density * Rs) / (Rs * (gas_surface_mole_density + oil_surface_mole_density * Rv));
		 auto V = zo / (1 - Kg) + zg / (1 - Ko);

		 if ( V<1 & V>0)  //Two-phase or both oil and gas saturated
		 {
			 //oil
		 	auto xo = oil_surface_mole_density / (oil_surface_mole_density + gas_surface_mole_density * Rs);
			auto xg = 1. - xo;
			oil_phase_model->ComputeSaturatedProperties(pressure, {xo,xg}, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));

			//gas
			auto yg = gas_surface_mole_density / (gas_surface_mole_density + oil_surface_mole_density * Rv);
			auto yo = 1. - yg;
			gas_phase_model->ComputeSaturatedProperties(pressure, { xg,xo }, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));
		 }
		 else if (V>1) //Only gas or undersaturated gas
		 {
			 //gas
			 gas_phase_model->ComputeUnderSaturatedProperties(Rv, pressure, {zg,zo}, out_variables.PhasesProperties.at(PHASE_TYPE::GAS));
		 }
		 else //Only oil or undersaturated oil
		 {
			 //oil
			 oil_phase_model->ComputeUnderSaturatedProperties(Rs, pressure, { zg,zo }, out_variables.PhasesProperties.at(PHASE_TYPE::OIL));
		 }




		

		
		 return true;





		//ASSERT(std::fabs(math::sum_array(feed) - 1.0) < 1e-12, "Feed sum must be 1");

		//const auto nbc = m_ComponentsProperties.NComponents;

		//auto& gas_comp = out_variables.PhasesProperties.at(PHASE_TYPE::GAS).MoleComposition.value = std::vector<double>(nbc, 0);
		//auto& oil_comp = out_variables.PhasesProperties.at(PHASE_TYPE::OIL).MoleComposition.value = std::vector<double>(nbc, 0);

		//auto& oil_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::OIL).value;
		//auto& gas_fraction = out_variables.PhaseMoleFraction.at(PHASE_TYPE::GAS).value;

		//auto KGasOil = ComputeWilsonGasOilKvalue(pressure, temperature);

		////Check for machine-zero feed values
		//const double epsilon = std::numeric_limits<double>::epsilon();
		//std::list<size_t> positive_components;
		//for (size_t i = 0; i != nbc; ++i)
		//{
		//	if (feed[i] > epsilon)
		//	{
		//		positive_components.push_back(i);
		//	}
		//}

		////Solve RachfordRice Equation
		//double vapor_fraction = 0;
		//vapor_fraction = SolveRachfordRiceEquation(KGasOil, feed, positive_components);
		//gas_fraction = vapor_fraction;
		//oil_fraction = 1.0 - gas_fraction;

		////Phase Mole Fractions
		//gas_comp = std::vector<double>(nbc, 0);
		//oil_comp = std::vector<double>(nbc, 0);
		//for (auto it = positive_components.begin(); it != positive_components.end(); ++it)
		//{
		//	gas_comp[*it] = feed[*it] / (1.0 + vapor_fraction * (KGasOil[*it] - 1.0));
		//	oil_comp[*it] = KGasOil[*it] * gas_comp[*it];
		//}
		//gas_comp = math::Normalize(gas_comp);
		//oil_comp = math::Normalize(oil_comp);

		////Retrieve physical bounds from negative flash values
		//if (gas_fraction >= 1.0)
		//{
		//	gas_fraction = 1.0;
		//	gas_comp = feed;
		//	oil_fraction = 0.0;
		//}
		//if (gas_fraction <= 0.0)
		//{
		//	gas_fraction = 0.0;
		//	oil_fraction = 1.0;
		//	oil_comp = feed;
		//}

		////Phase Properties
		//for (auto& m_PhaseModel : out_variables.PhaseModels)
		//{
		//	auto& comp = out_variables.PhasesProperties.at(m_PhaseModel.first).MoleComposition.value;
		//	m_PhaseModel.second->ComputeAllProperties(pressure, temperature, comp, out_variables.PhasesProperties[m_PhaseModel.first]);
		//}

		////Compute Phase State
		//set_PhaseState(out_variables);


	}

}


