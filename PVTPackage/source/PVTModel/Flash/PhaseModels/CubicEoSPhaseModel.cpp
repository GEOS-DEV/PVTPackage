#include <algorithm>
#include "CubicEoSPhaseModel.hpp"
#include "Utils/Logger.hpp"
#include "PVTModel/ComponentProperties.hpp"

namespace PVTPackage
{

	CubicEosMixtureCoefficients CubicEoSPhaseModel::ComputeMixtureCoefficients( double Pressure, double Temperature, std::vector<double>& composition)
	{
		auto nbc = m_ComponentsProperties->NComponents;
		auto& Tc = m_ComponentsProperties->Tc;
		auto& Pc = m_ComponentsProperties->Pc;

		CubicEosMixtureCoefficients mix_coeffs(nbc);

		//Mixture coefficients
		for (size_t i = 0; i < nbc; ++i)
		{
			mix_coeffs.APure[i] = m_OmegaA * Tc[i] * Tc[i] * Pressure / (Pc[i] * Temperature*Temperature) *  pow(1.0 + m_m[i] * (1.0 - sqrt(Temperature / Tc[i])), 2.0);
			mix_coeffs.BPure[i] = m_OmegaB * Tc[i] * Pressure / (Pc[i] * Temperature);
		}

		mix_coeffs.AMixture = 0;
		mix_coeffs.BMixture = 0;
		for (size_t i = 0; i < nbc; ++i)
		{
			for (size_t j = 0; j < nbc; ++j)
			{
				mix_coeffs.AMixture = mix_coeffs.AMixture + (composition[i] * composition[j] * (1.0 - m_BICs) * sqrt(mix_coeffs.APure[i] * mix_coeffs.APure[j]));
			}
			mix_coeffs.BMixture = mix_coeffs.BMixture + composition[i] * mix_coeffs.BPure[i];
		}

		return mix_coeffs;

	}

	double CubicEoSPhaseModel::ComputeCompressibilityFactor(double Pressure, double Temperature, std::vector<double>& composition, CubicEosMixtureCoefficients& mix_coeffs) const
	{
		//ASSERT(m_MixtureCoefficientsUpToDate, "Z factor requires mixture properties up-to-date.");
		std::vector<double> coeff(3);
		//aZ3+bZ2+cZ+d=0
		double a = 1.0;
		double b = (m_Delta1 + m_Delta2 - 1.0)*mix_coeffs.BMixture - 1.0;
		double c = mix_coeffs.AMixture + m_Delta1*m_Delta2*mix_coeffs.BMixture*mix_coeffs.BMixture - (m_Delta1 + m_Delta2)*mix_coeffs.BMixture*(mix_coeffs.BMixture + 1.0);
		double d = -(mix_coeffs.AMixture*mix_coeffs.BMixture + m_Delta1*m_Delta2*mix_coeffs.BMixture*mix_coeffs.BMixture*(mix_coeffs.BMixture + 1.0));

		auto sols = SolveCubicPolynomial(a, b, c, d);
		double compressibility;


		if ((m_PhaseType == PHASE_TYPE::LIQUID_WATER_RICH) || (m_PhaseType == PHASE_TYPE::OIL))
		{
			compressibility = *(std::min_element(sols.begin(), sols.end()));
		}
		else if (m_PhaseType == PHASE_TYPE::GAS)
		{
			compressibility = *(std::max_element(sols.begin(), sols.end()));
		}
		else
		{
			LOGERROR("Undetermined phase type for compressibility calculation");
			compressibility = -1;
		}

		return compressibility;

	}

	double CubicEoSPhaseModel::ComputeMoleDensity_(double Pressure, double Temperature, std::vector<double>& composition, double Z) const
	{
		auto nbc = m_ComponentsProperties->NComponents;
		auto& Vs = m_ComponentsProperties->VolumeShift;
		double v_eos = R * Temperature * Z / Pressure;
		double v_corrected = v_eos;

		double mole_density;

		std::vector<double> vshift;
		for (size_t i = 0; i < nbc; i++)
		{
			v_corrected = v_corrected + composition[i] * ((Vs[i][1]) * Temperature + Vs[i][1]);
		}

		if (v_corrected != 0)
		{
			mole_density = 1.0 / v_corrected;
		}
		else
		{
			mole_density = 0.0;
		}

		return mole_density;
	}

	CubicEoSPhaseModelProperties CubicEoSPhaseModel::ComputeMoleDensity(double Pressure, double Temperature, std::vector<double>& composition)
	{
		auto mix_coeffs = ComputeMixtureCoefficients(Pressure, Temperature, composition);
		auto Z = ComputeCompressibilityFactor(Pressure, Temperature, composition, mix_coeffs);
		auto mol_dens = ComputeMoleDensity_(Pressure, Temperature, composition,Z);

		return CubicEoSPhaseModelProperties(Z,mol_dens,{});
	}

	CubicEoSPhaseModelProperties CubicEoSPhaseModel::ComputeFugacities(double Pressure, double Temperature, std::vector<double>& composition)
	{
		auto mix_coeffs = ComputeMixtureCoefficients(Pressure, Temperature, composition);
		auto Z = ComputeCompressibilityFactor(Pressure, Temperature, composition,mix_coeffs);
		auto ln_phi = ComputeLnFugacitiesCoefficients(Pressure, Temperature, composition,Z,mix_coeffs);
		return CubicEoSPhaseModelProperties(Z, 0, ln_phi);
	}

	CubicEoSPhaseModelProperties CubicEoSPhaseModel::ComputeAllProperties(double Pressure, double Temperature, std::vector<double>& composition)
	{
		auto mix_coeffs = ComputeMixtureCoefficients(Pressure, Temperature, composition);
		auto Z = ComputeCompressibilityFactor(Pressure, Temperature, composition, mix_coeffs);
		auto ln_phi = ComputeLnFugacitiesCoefficients(Pressure, Temperature, composition, Z, mix_coeffs);
		auto mol_dens = ComputeMoleDensity_(Pressure, Temperature, composition, Z);
		return CubicEoSPhaseModelProperties(Z, mol_dens, ln_phi);
	}

	std::vector<double> CubicEoSPhaseModel::ComputeLnFugacitiesCoefficients(double Pressure, double Temperature, std::vector<double>& composition, double Z, CubicEosMixtureCoefficients& mix_coeffs) const
	{
		auto nbc = m_ComponentsProperties->NComponents;

		auto ki = std::vector<double>(nbc, 0);

		std::vector<double> ln_fugacity_coeffs(nbc);

		//Ki
		for (size_t i = 0; i < nbc; ++i)
		{
			for (size_t j = 0; j < nbc; ++j)
			{
				ki[i] = ki[i] + composition[j] * (1.0 - m_BICs) * sqrt(mix_coeffs.APure[i] * mix_coeffs.APure[j]);
			}
		}
	
		//E
		double E = (Z + m_Delta1*mix_coeffs.BMixture) / (Z + m_Delta2*mix_coeffs.BMixture);

		//Ln phi
		for (size_t i = 0; i < nbc; ++i)
		{
			ln_fugacity_coeffs[i] = (Z - 1)*mix_coeffs.BPure[i] / mix_coeffs.BMixture - log(Z - mix_coeffs.BMixture)
				- 1 / ((m_Delta1 - m_Delta2)*mix_coeffs.BMixture) * (2 * ki[i] - mix_coeffs.AMixture + mix_coeffs.BPure[i] / mix_coeffs.BMixture) * log(E);
		}

		return ln_fugacity_coeffs;

	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void CubicEoSPhaseModel::Init()
	{
		auto nbc = m_ComponentsProperties->NComponents;
		auto& omega = m_ComponentsProperties->Omega;

		//Set EOS constants
		switch (m_EOSType)
		{
		case EOS_TYPE::PENG_ROBINSON:
			m_OmegaA = 0.457235529;
			m_OmegaB = 0.077796074;
			m_Delta1 = 1 + sqrt(2);
			m_Delta2 = 1 - sqrt(2);
			EOS_m_function = &CubicEoSPhaseModel::m_function_PR;
			break;
		case EOS_TYPE::REDLICH_KWONG_SOAVE:
			m_OmegaA = 0.42748;
			m_OmegaB = 0.08664;
			m_Delta1 = 0.0;
			m_Delta2 = 1;
			EOS_m_function = &CubicEoSPhaseModel::m_function_SRK;
			break;
		default:
			LOGERROR("non supported equation of state");
			break;
		}

		//Set Constant properties
		m_aTc.resize(nbc, 0);
		m_bTc.resize(nbc, 0);
		m_m.resize(nbc, 0);

		for (size_t i = 0; i < nbc; i++)
		{
			m_m[i] = (this->*EOS_m_function)(omega[i]);

		}
	}

	/**
	* \brief Solve Cubic Polynomial
	* \param coeff
	* \return
	*/
	std::vector<double> CubicEoSPhaseModel::SolveCubicPolynomial(double m3, double m2, double m1, double m0) const
	{
		////CUBIC EQUATION :  m3 * x^3 +  m3 * x^2 + m1 *x + m0  = 0
		double x1, x2, x3;

		double a1 = m2 / m3;
		double a2 = m1 / m3;
		double a3 = m0 / m3;

		double Q = (a1 * a1 - 3 * a2) / 9;
		double R = (2 * a1 * a1 * a1 - 9 * a1 * a2 + 27 * a3) / 54;
		double Qcubed = Q * Q * Q;
		double d = Qcubed - R * R;

		/* Three real roots */
		if (d >= 0)
		{
			double theta = acos(R / sqrt(Qcubed));
			double sqrtQ = sqrt(Q);
			x1 = -2 * sqrtQ * cos(theta / 3) - a1 / 3;
			x2 = -2 * sqrtQ * cos((theta + 2 * PI) / 3) - a1 / 3;
			x3 = -2 * sqrtQ * cos((theta + 4 * PI) / 3) - a1 / 3;
			return{ x1,x2,x3 };

		}
		/* One real root */
		else
		{
			double e = pow(sqrt(-d) + fabs(R), 1. / 3.);
			if (R > 0)
				e = -e;
			x1 = (e + Q / e) - a1 / 3.;
			return{ x1 };
		}
	}

	double CubicEoSPhaseModel::m_function_PR(double omega)
	{
		if (omega < 0.49)
		{
			return 0.37464 + 1.54226 * omega - 0.26992 * omega * omega;
		}
		else
		{
			return 0.3796 + 1.485 * omega - 0.164423 * omega * omega + 0.016666 * omega * omega * omega;
		}
	}

	double CubicEoSPhaseModel::m_function_SRK(double omega)
	{
		return 0.480 + 1.574 * omega - 0.176 * omega * omega;
	}
}
