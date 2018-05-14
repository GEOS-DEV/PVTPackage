#include "PVTModel/ComponentProperties.hpp"
#include "PVTModel/CompositionalPVTModel.hpp"
#include <chrono>
#include "PVTModel/FlashVariables.hpp"
#include <ctime>
#include "PVTModel/PhaseSplitModelOutputVariables.hpp"
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
	PhaseSplitModelOutputVariables Flash_out;

	auto Model = CompositionalPVTModel({ PHASE_TYPE::OIL,PHASE_TYPE::GAS,PHASE_TYPE::LIQUID_WATER_RICH }, { EOS_TYPE::PENG_ROBINSON,EOS_TYPE::PENG_ROBINSON,EOS_TYPE::PENG_ROBINSON }, COMPOSITIONAL_FLASH_TYPE::TRIVIAL, CompProps);
	
	for (size_t nb = 0; nb != 100; ++nb)
	{
		Model.Flash(1e5,300, {0.1,0.1,0.25,0.55}, Flash_out);
	}

	end = std::chrono::system_clock::now();

	double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();

	std::cout << "elapsed time: " << elapsed_seconds/1000 << "s\n";

	return 0;
}


