#pragma once
#include "Utils/Assert.hpp"
#include "MultiphaseSystem/PVTEnums.hpp"
#include <vector>
#include <cmath>
#include "MultiphaseSystem/PhaseModel/PhaseModel.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"


namespace PVTPackage
{
	struct CubicEoSPhaseProperties;

	class CubicEoSPhaseModel final : public PhaseModel
	{
	public:

		CubicEoSPhaseModel(ComponentProperties comp_props, EOS_TYPE eos, PHASE_TYPE phase_type)
			: PhaseModel(), m_ComponentsProperties(comp_props),m_PhaseType(phase_type),
			  m_EOSType(eos), m_OmegaA(0), m_OmegaB(0), m_Delta1(0), m_Delta2(0), EOS_m_function(nullptr),
			  m_BICs(0)
		{
			Init();
		}

		~CubicEoSPhaseModel() override = default;

		const ComponentProperties& get_ComponentsProperties() 
		{
			return m_ComponentsProperties;
		}

		void ComputeAllProperties(double Pressure, double Temperature, std::vector<double>& composition, PhaseProperties* props_out) override;

	protected:

		//Component Properties
		ComponentProperties m_ComponentsProperties;

		//Constants
		const double R = 8.3144621;
		const double PI = std::acos(-1);

		//Phase Type
		PHASE_TYPE m_PhaseType;

		//EOS parameters
		EOS_TYPE m_EOSType;
		double m_OmegaA;
		double m_OmegaB;
		double m_Delta1, m_Delta2;
		double (CubicEoSPhaseModel::*EOS_m_function)(double);

		//Constant Properties
		std::vector<double> m_m;
		double m_BICs;

		//Init function at instatiation
		void Init();

		//Used to shorten input
		struct CubicEosMixtureCoefficients
		{
			CubicEosMixtureCoefficients(size_t nc) : APure(std::vector<double>(nc)), BPure(std::vector<double>(nc)),
				AMixture(0), BMixture(0)
			{
			}

			std::vector<double> APure, BPure;
			double AMixture, BMixture;
		};


		CubicEosMixtureCoefficients ComputeMixtureCoefficients(double Pressure, double Temperature, std::vector<double>& composition);
		double ComputeCompressibilityFactor( double Pressure, double Temperature, std::vector<double>& composition, CubicEosMixtureCoefficients& mix_coeffs) const;
		double ComputeMoleDensity_(double Pressure, double Temperature, std::vector<double>& composition, double Z) const;
		double ComputeMolecularWeight(std::vector<double>& composition) const;
		double ComputeMassDensity_(double mole_density, double mw) const;
		std::vector<double> ComputeLnFugacitiesCoefficients( double Pressure, double Temperature, std::vector<double>& composition, double Z, CubicEosMixtureCoefficients& mixture_coefficients) const;

		//
		std::vector<double> SolveCubicPolynomial(double a, double b, double c, double d) const;

		//m functions
		double m_function_PR(double omega);
		double m_function_SRK(double omega);



	};
}
