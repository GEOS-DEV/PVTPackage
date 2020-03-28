#include "MultiphaseSystem/PhaseSplitModel/CompositionalFlash.hpp"
#include "MultiphaseSystem/ComponentProperties.hpp"
#include  "MultiphaseSystem/PhaseModel/CubicEOS/CubicEoSPhaseModel.hpp"
#include "MultiphaseSystem/MultiphaseSystemProperties.hpp"
#include "Utils/math.hpp"
#include <algorithm>

namespace PVTPackage
{

double CompositionalFlash::SolveRachfordRiceEquation(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index)
{
  double gas_phase_mole_fraction=0;

  //Numerical Parameters //TODO: move them outside the function
  double SSI_tolerance=1e-3;
  int max_SSI_iterations = 200;
  double Newton_tolerance = 1e-12;
  int max_Newton_iterations = 30;
  double epsilon = std::numeric_limits<double>::epsilon();

  //Min and Max Kvalues for non-zero composition
  double max_K=0, min_K=1/epsilon;
  for (auto it = non_zero_index.begin(); it != non_zero_index.end(); ++it)
  {
    if (Kvalues[*it] > max_K)
      max_K = Kvalues[*it];
    if (Kvalues[*it] < min_K)
      min_K = Kvalues[*it];
  }

  //Check for trivial solutions. This corresponds to bad Kvalues //TODO:to be fixed
  if (max_K < 1.0)
  {
    return gas_phase_mole_fraction = 0.0;
  }
  if (min_K > 1.0)
  {
    return gas_phase_mole_fraction = 1.0;
  }

  //Solver
  //Find solution window
  double x_min = 1.0 / (1 - max_K);
  double x_max = 1.0 / (1 - min_K);
  double sqrt_epsilon = sqrt(epsilon);
  x_min = x_min + sqrt_epsilon * (std::fabs(x_min) + sqrt_epsilon);
  x_max = x_max - sqrt_epsilon * (std::fabs(x_max) + sqrt_epsilon);

  double current_error = 1 / epsilon;

  //SSI loop
  double func_x_min =0, func_x_mid =0, func_x_max =0;
  bool recompute_min = true, recompute_max = true;
  int SSI_iteration=0;
  while ((current_error > SSI_tolerance)&&(SSI_iteration<max_SSI_iterations))
  {
    double x_mid = 0.5*(x_min + x_max);
    if (recompute_min) func_x_min = RachfordRiceFunction(Kvalues, feed, non_zero_index, x_min);
    if (recompute_max) func_x_max = RachfordRiceFunction(Kvalues, feed, non_zero_index, x_max);
    func_x_mid = RachfordRiceFunction(Kvalues, feed, non_zero_index, x_mid);

    if ((func_x_min < 0) && (func_x_max < 0))
    {
      return gas_phase_mole_fraction = 0.0;
    }
    else if ((func_x_min > 1) && (func_x_max > 1))
    {
      return gas_phase_mole_fraction = 1.0;
    }
    else if (func_x_min*func_x_mid <0.0)
    {
      x_max = x_mid;
      recompute_max = true;
      recompute_min = false;
    }
    else if (func_x_max*func_x_mid < 0.0)
    {
      x_min = x_mid;
      recompute_max = false;
      recompute_min = true;
    }

    current_error = std::min(std::fabs(func_x_max - func_x_min),std::fabs(x_max - x_min));
    SSI_iteration++;

    if (SSI_iteration == max_SSI_iterations)
      LOGWARNING("Rachford-Rice SSI reached max number of iterations");
  }

  gas_phase_mole_fraction = 0.5*(x_max + x_min);

  //Newton loop
  int Newton_iteration = 0;
  double Newton_value = gas_phase_mole_fraction;
  while ((current_error > Newton_tolerance)&&(Newton_iteration<max_Newton_iterations))
  {
    double delta_Newton = -RachfordRiceFunction(Kvalues, feed, non_zero_index, Newton_value) / dRachfordRiceFunction_dx(Kvalues, feed, non_zero_index, Newton_value);
    current_error = std::fabs(delta_Newton) / std::fabs(Newton_value);

    // Test if we are stepping out of the [x_min;x_max] interval
    if (Newton_value + delta_Newton < x_min)
    {
      Newton_value = .5 * (Newton_value + x_min);
    }
    else if (Newton_value + delta_Newton > x_max)
    {
      Newton_value = .5 * (Newton_value + x_max);
    }
    else
    {
      Newton_value = Newton_value + delta_Newton;
    }

    Newton_iteration++;

    if (Newton_iteration == max_Newton_iterations)
      LOGWARNING("Rachford-Rice Newton reached max number of iterations");
  }
  return gas_phase_mole_fraction = Newton_value;

}

double CompositionalFlash::RachfordRiceFunction(const std::vector<double>& Kvalues, const std::vector<double>& feed,const std::list<size_t>& non_zero_index, double x)
{
  double val = 0;
  for (auto ic : non_zero_index)
  {
    const double K = (Kvalues[ic] - 1.0);
    val = val + feed[ic] * K / (1.0 + x * K);
  }
  return val;
}

double CompositionalFlash::dRachfordRiceFunction_dx(const std::vector<double>& Kvalues, const std::vector<double>& feed, const std::list<size_t>& non_zero_index, double x)
{
  double val = 0;
  for (auto ic : non_zero_index)
  {
    const double K = (Kvalues[ic] - 1.0);
    const double R = K / (1.0 + x * K);
    val = val - feed[ic] * R * R;
  }
  return val;
}

std::vector<double> CompositionalFlash::ComputeWilsonGasLiquidKvalue(double Pressure, double Temperature) const
{
  const auto nbc = m_ComponentsProperties.NComponents;
  const auto& Tc = m_ComponentsProperties.Tc;
  const auto& Pc = m_ComponentsProperties.Pc;
  const auto& Omega = m_ComponentsProperties.Omega;

  std::vector<double> Kval(nbc);

  //Gas-Oil
  for (size_t i = 0; i != nbc; i++)
  {
    Kval[i] = Pc[i] / Pressure * exp(5.37*(1 + Omega[i])*(1 - Tc[i] / Temperature));
  }

  return Kval;
}

std::vector<double> CompositionalFlash::ComputeWaterGasKvalue(double Pressure, double Temperature) const
{
  const auto nbc = m_ComponentsProperties.NComponents;
  const auto water_index = m_ComponentsProperties.WaterIndex;
  std::vector<double> Kval(nbc, 0);
  Kval[water_index] = exp(-4844.168051 / Temperature + 12.93022442)*1e5 / Pressure;
  return Kval;

}

std::vector<double> CompositionalFlash::ComputeWaterOilKvalue(double Pressure, double Temperature) const
{
  (void) Pressure, (void) Temperature;
  const auto nbc = m_ComponentsProperties.NComponents;
  return std::vector<double>(nbc, 0);
}

//void CompositionalFlash::ComputeFiniteDifferenceDerivatives(MultiphaseSystemProperties& out_variables)
//{
//	const auto& pressure = out_variables.Pressure;
//	const auto& temperature = out_variables.Temperature;
//	const auto& feed = out_variables.Feed;

//	auto  sqrtprecision = sqrt(std::numeric_limits<double>::epsilon());
//	double epsilon = 0;

//	MultiphaseSystemProperties props_eps = out_variables;

//	////Pressure
//	epsilon = sqrtprecision * (std::fabs(pressure) + sqrtprecision);
//	props_eps.Pressure = pressure + epsilon;
//	ComputeEquilibrium(props_eps);
//	props_eps.Pressure = pressure;
//	out_variables.UpdateDerivative_dP_FiniteDifference(props_eps, epsilon);

//	////Temperature
//	epsilon = sqrtprecision * (std::fabs(temperature) + sqrtprecision);
//	props_eps.Temperature = temperature + epsilon;
//	ComputeEquilibrium(props_eps);
//	props_eps.Temperature = temperature;
//	out_variables.UpdateDerivative_dT_FiniteDifference(props_eps, epsilon);

//	//Feed
//	for (size_t i=0; i<feed.size();++i)
//	{
//		epsilon = sqrtprecision * (std::fabs(feed[i]) + sqrtprecision);
//		if (feed[i] + epsilon > 1)
//		{
//			epsilon = -epsilon;
//		}
//		auto save_feed = feed;
//		props_eps.Feed[i] = feed[i] + epsilon;
//		props_eps.Feed = math::Normalize(props_eps.Feed);
//		ComputeEquilibrium(props_eps);
//		props_eps.Feed = save_feed;
//		out_variables.UpdateDerivative_dz_FiniteDifference(i,props_eps, epsilon);
//	}

//}

}


