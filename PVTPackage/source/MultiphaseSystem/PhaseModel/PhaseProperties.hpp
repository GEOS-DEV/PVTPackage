#pragma once
#include "MultiphaseSystem/PropertyAndDerivatives.hpp"


namespace PVTPackage
{

	struct PhaseProperties
	{
		PhaseProperties(size_t ncomponents) : MolecularWeight(ncomponents), CompressibilityFactor(ncomponents), MoleComposition(ncomponents,ncomponents), LnFugacityCoefficients(ncomponents, ncomponents),
		                    MoleDensity(ncomponents),
		                    MassDensity(ncomponents), Viscosity(0),
		                    MassEnthalpy(0), Compressibility(0)
		{
		}

		void UpdateDerivative_dP_FiniteDifference(PhaseProperties& props_eps, double epsilon)
		{
			CompressibilityFactor.dP = (props_eps.CompressibilityFactor.value - CompressibilityFactor.value) / epsilon;
			MoleDensity.dP = (props_eps.MoleDensity.value - MoleDensity.value) / epsilon;
			MassDensity.dP = (props_eps.MassDensity.value - MassDensity.value) / epsilon;
			for (size_t i = 0; i < MoleComposition.value.size(); ++i)
			{
				MoleComposition.dP[i] = (props_eps.MoleComposition.value[i] - MoleComposition.value[i]) / epsilon;
				LnFugacityCoefficients.dP[i] = (props_eps.LnFugacityCoefficients.value[i] - LnFugacityCoefficients.value[i]) / epsilon;
			}
		}

		void UpdateDerivative_dT_FiniteDifference(const PhaseProperties& props_eps, double epsilon)
		{
			CompressibilityFactor.dT = (props_eps.CompressibilityFactor.value - CompressibilityFactor.value) / epsilon;
			MoleDensity.dT = (props_eps.MoleDensity.value - MoleDensity.value) / epsilon;
			MassDensity.dT = (props_eps.MassDensity.value - MassDensity.value) / epsilon;
			for (size_t i = 0; i < MoleComposition.value.size(); ++i)
			{
				MoleComposition.dT[i] = (props_eps.MoleComposition.value[i] - MoleComposition.value[i]) / epsilon;
				LnFugacityCoefficients.dT[i] = (props_eps.LnFugacityCoefficients.value[i] - LnFugacityCoefficients.value[i]) / epsilon;
			}
		}

		void UpdateDerivative_dz_FiniteDifference(size_t icomponent, const PhaseProperties& props_eps, double epsilon)
		{
				CompressibilityFactor.dz[icomponent] = (props_eps.CompressibilityFactor.value - CompressibilityFactor.value) / epsilon;
				MoleDensity.dz[icomponent] = (props_eps.MoleDensity.value - MoleDensity.value) / epsilon;
				MassDensity.dz[icomponent] = (props_eps.MassDensity.value - MassDensity.value) / epsilon;
				for (size_t i = 0; i < MoleComposition.value.size(); ++i)
				{
					MoleComposition.dz[i][icomponent] = (props_eps.MoleComposition.value[i] - MoleComposition.value[i]) / epsilon;
					LnFugacityCoefficients.dz[i][icomponent] = (props_eps.LnFugacityCoefficients.value[i] - LnFugacityCoefficients.value[i]) / epsilon;
				}
		}


		//Properties
		//--Constant
		ScalarPropertyAndDerivatives<double> MolecularWeight;
		//--From Flash
		ScalarPropertyAndDerivatives<double> CompressibilityFactor;
		VectorPropertyAndDerivatives<double> MoleComposition;
		VectorPropertyAndDerivatives<double> LnFugacityCoefficients;
		//--Indirectly from flash
		ScalarPropertyAndDerivatives<double> MoleDensity;
		ScalarPropertyAndDerivatives<double> MassDensity;
		
		double Viscosity;
		double MassEnthalpy;
		double Compressibility;
		
	};


}
