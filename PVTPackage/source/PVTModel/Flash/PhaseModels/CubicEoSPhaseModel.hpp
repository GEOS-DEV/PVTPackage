#pragma once
#include "Utils/Assert.hpp"
#include "PVTModel/PVTEnums.hpp"
#include "PVTModel/ComponentProperties.hpp"
#include <vector>

struct ComponentProperties;

namespace PVTPackage
{

	struct CubicEosMixtureCoefficients
	{
		CubicEosMixtureCoefficients(size_t nc): APure(std::vector<double>(nc)), BPure(std::vector<double>(nc)), AMixture(0), BMixture(0)
		{
		}
		std::vector<double> APure, BPure;
		double AMixture, BMixture;
	};

	struct CubicEoSPhaseModelProperties
	{
		CubicEoSPhaseModelProperties(double Z, double rho_mol, std::vector<double> ln_phi):CompressibilityFactor(Z),MoleDensity(rho_mol),LnFugacityCoefficients(ln_phi), ln_phi_(std::move(ln_phi)) {}
		double CompressibilityFactor;
		double MoleDensity;
		std::vector<double> LnFugacityCoefficients;
		std::vector<double> ln_phi_;
	};


	class CubicEoSPhaseModel
	{
	public:

		//CubicEoSPhaseModel():m_ComponentsProperties()
		//{
		//	Init();
		//}

		CubicEoSPhaseModel(const ComponentProperties* component_properties, EOS_TYPE eos, PHASE_TYPE phase_type)
			: m_ComponentsProperties(component_properties), m_PhaseType(phase_type),
			  m_EOSType(eos), m_OmegaA(0), m_OmegaB(0), m_Delta1(0), m_Delta2(0), EOS_m_function(nullptr),
			  m_BICs(0)
		{
			Init();
		}

		CubicEoSPhaseModelProperties ComputeMoleDensity(double Pressure, double Temperature, std::vector<double>& composition);
		CubicEoSPhaseModelProperties ComputeFugacities(double Pressure, double Temperature, std::vector<double>& composition);
		CubicEoSPhaseModelProperties ComputeAllProperties(double Pressure, double Temperature, std::vector<double>& composition);

		////Getter
		//std::vector<double> get_LnFugacitiesCoefficient() { return m_LnFugacitiesCoefficient; }
		//double get_MoleDensity() { return m_MoleDensity; }

	protected:

		//Components Properties
		const ComponentProperties* m_ComponentsProperties;

		//Constants
		const double R = 8.3144621;
		const double PI = acos(-1);

		//Phase Type
		PHASE_TYPE m_PhaseType;

		//EOS parameters
		EOS_TYPE m_EOSType;
		double m_OmegaA;
		double m_OmegaB;
		double m_Delta1, m_Delta2;
		double (CubicEoSPhaseModel::*EOS_m_function)(double);

		//Constant Properties
		std::vector<double> m_aTc, m_bTc, m_m;
		double m_BICs;

		//
		void Init();
		CubicEosMixtureCoefficients ComputeMixtureCoefficients(double Pressure, double Temperature, std::vector<double>& composition);
		double ComputeCompressibilityFactor( double Pressure, double Temperature, std::vector<double>& composition, CubicEosMixtureCoefficients& mix_coeffs) const;
		double ComputeMoleDensity_(double Pressure, double Temperature, std::vector<double>& composition, double Z) const;
		std::vector<double> ComputeLnFugacitiesCoefficients( double Pressure, double Temperature, std::vector<double>& composition, double Z, CubicEosMixtureCoefficients& mixture_coefficients) const;


		//
		std::vector<double> SolveCubicPolynomial(double a, double b, double c, double d) const;

		//m functions
		double m_function_PR(double omega);
		double m_function_SRK(double omega);



	};
}
