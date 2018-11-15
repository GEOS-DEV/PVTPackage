#include "MultiphaseSystem/ComponentProperties.hpp"
#include <chrono>
#include <vector>
#include "MultiphaseSystem/BlackOilMultiphaseSystem.hpp"
//#include <cstdlib>
//#include <vld.h> //Visual leak detector

int main(int argc, const char * argv[]) 
{

	using namespace PVTPackage;



	/// ------------------------------ BO RAW DATASET

	//PVTO
	std::vector<std::vector<double>> PVTO =
	{
	//	Rs[sm3/sm3]		Pbub[bar]	Bo[m3/sm3]	Visc(cP)
		{  2,	20,		1.02,	0.975	},
		{  5,	50,		1.03,	0.91	},
		{ 10,	100,	1.04,	0.83	},
		{ 15,	200,	1.05,	0.695	},
		{		900,	1.03,	0.985   },
		{ 30,	300,	1.07,	0.594	},
		{ 40,	400,	1.08,	0.51	},
		//P
		{		500,	1.07,	0.549	},
		{		900,	1.06,	0.74	},
		{ 50,	500.7,	1.09,	0.449	},
		//P
		{		900.7,	1.08,	0.605	}
	};

	//PVTW
	std::vector<double> PVTW =
	{
		//	Pref[bar]	Bw[m3/sm3]	Cp[1/bar]	Visc[cP]
			306.1,	1.03,	0.000041,	0.3
	};

	//PVTG
	std::vector<std::vector<double>> PVTG =
	{
		//Pg(bar)	Rv(sm3/sm3)	Bg(m3/sm3)	Visc(cP)

		{	30,		0.000132,	0.04234,	0.01344	},
		{			0,			0.04231,	0.01389	},
		{	60,		0.000124,	0.02046,	0.0142	},
		{			0,			0.02043,	0.0145	},
		{	90,		0.000126,	0.01328,	0.01526	},
		{			0,			0.01325,	0.01532	},
		{	120,	0.000135,	0.00977,	0.0166	},
		{			0,			0.00973,	0.01634	},
		{	150,	0.000149,	0.00773,	0.01818	},
		{			0,			0.00769,	0.01752	},
		{	180,	0.000163,	0.006426,	0.01994	},
		{			0,			0.006405,	0.01883	},
		{	210,	0.000191,	0.005541,	0.02181	},
		{			0,			0.005553,	0.02021	},
		{	240,	0.000225,	0.004919,	0.0237	},
		{			0,			0.004952,	0.02163	},
		{	270,	0.000272,	0.004471,	0.02559	},
		{			0,			0.004511,	0.02305	},
		{	295,	0.000354,	0.004194,	0.02714	},
		{			0,			0.004225,	0.02423	},
		{	310,	0.000403,	0.004031,	0.02806	},
		{			0.000354,	0.004059,	0.02768	},
		{	330,	0.000354,	0.00391,	0.02832	},
		{ 			0,			0.003913,	0.02583	},
		{	530,	0.000479,	0.003868,	0.02935	},
		{			0.000354,	0.0039,		0.02842	},
		{			0,			0.003903,	0.02593	}
	};

	//Surface Density [Kg/m3]
	double SurfaceOilDensity = 800e-3;
	double SurfaceWaterDensity = 1022e-3;
	double SurfaceGasDensity = 0.9907e-3;

	//Molecular Weight [Kg/mol]
	double OilMw = 114e-3;
	double WaterMw = 18e-3;
	double GasMw = 16e-3;

	/// ------------------------------  END BO RAW DATASET
	auto PVTSystem = BlackOilMultiphaseSystem({ PHASE_TYPE::OIL,PHASE_TYPE::GAS,PHASE_TYPE::LIQUID_WATER_RICH }, PVTO, PVTW, PVTG, { SurfaceOilDensity ,SurfaceWaterDensity ,SurfaceGasDensity }, { OilMw ,WaterMw ,GasMw });

	//Domain
	const size_t NBlocks = static_cast<size_t>(1e1);
	std::vector<double> Pressure(NBlocks), Temperature(NBlocks);
	std::vector<std::vector<double>> Feed(NBlocks, {0.99,0.01,0});
	srand(0);
	for (size_t nb = 0; nb != NBlocks; ++nb)
	{
		Pressure[nb] = 1e5; //rand() % 200000 + 100000;
		Temperature[nb] = 300;//rand() % 400 + 300;
	}

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();


	for (size_t nb = 0; nb != NBlocks; ++nb)
	{
		PVTSystem.Update(Pressure[nb], Temperature[nb], Feed[nb]);
	}

	end = std::chrono::system_clock::now();

	auto PhaseRepartitionProperties = PVTSystem.get_MultiphaseSystemProperties();
	auto OilProperties = PVTSystem.get_PhaseProperties(PHASE_TYPE::OIL);

	double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count();

	std::cout << "elapsed time: " << elapsed_seconds / 1000 << "s\n";

	return 0;
}


