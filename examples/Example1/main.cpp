#include "PVTModel/ComponentProperties.hpp"
#include "PVTModel/CompositionalPVTModel.hpp"
#include <chrono>
#include "PVTModel/FlashVariables.hpp"
#include <ctime>
//#include <vld.h> //Visual leak detector

int main(int argc, const char * argv[]) 
{
	std::chrono::time_point<std::chrono::system_clock> start, end;

	start = std::chrono::system_clock::now();

	using namespace PVTPackage;

	std::vector<std::string> Labels = { "N2","C10","C20","H2O" };
	auto Pc = { 34e5,25.3e5,14.6e5,220.5e5 };
	auto Tc = { 126.2,622.0,782.0,647.0 };
	auto Omega = { 0.04,0.443,0.816,0.344 };
	auto Mw = { 28e-3,134e-3,275e-3,18e-3 };
	auto nbc = Pc.size();
	const ComponentProperties CompProps(nbc, Labels, Mw, Tc, Pc, Omega);
	FlashInputVariables Flash_in(nbc);
	FlashOutputVariables Flash_out(&Flash_in);

	auto Model = CompositionalPVTModel({ PHASE_TYPE::OIL,PHASE_TYPE::GAS,PHASE_TYPE::LIQUID_WATER_RICH }, { EOS_TYPE::PENG_ROBINSON,EOS_TYPE::PENG_ROBINSON,EOS_TYPE::PENG_ROBINSON }, COMPOSITIONAL_FLASH_TYPE::TRIVIAL, CompProps);
	
	Flash_in.SetFeed({ 0.1,0.1,0.25,0.55 });
	Flash_in.SetTemperature(300);
	for (size_t nb = 0; nb != 10; ++nb)
	{
		Flash_in.SetPressure(1e5);
		Model.Flash(Flash_in, Flash_out);
	}

	end = std::chrono::system_clock::now();

	double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();

	std::cout << "elapsed time: " << elapsed_seconds/1000 << "s\n";


	//Compute Mass Density
	std::vector<double> oil_mass_density, gas_mass_density;
	
	double Mwo = 0, Mwg = 0;
	auto comp_oil = Flash_out.get_Composition(PHASE_TYPE::OIL);
	auto comp_gas = Flash_out.get_Composition(PHASE_TYPE::GAS);
	auto comp_water = Flash_out.get_Composition(PHASE_TYPE::LIQUID_WATER_RICH);
	for (size_t nc = 0; nc != CompProps.NComponents; ++nc)
	{
		Mwo = Mwo + comp_oil[nc] * CompProps.Mw[nc];
		Mwg = Mwg + comp_gas[nc] * CompProps.Mw[nc];
	}
	oil_mass_density.push_back(Flash_out.get_MoleDensity(PHASE_TYPE::OIL) * Mwo);
	gas_mass_density.push_back(Flash_out.get_MoleDensity(PHASE_TYPE::GAS) * Mwg);


	return 0;
}


