/* 
 * File: Table.h
 * --------------
 * Table lookup Class
 * ----------------------------------------------------
 * Copied from GPRS
 *		
 */
#pragma once

 // Std library includes
#include <vector>
#include <cmath>

namespace PVTPackage
{

	class Table {
	public:
		Table() {}
		~Table() {}

		//vector implementation
		// --- 2 column table ----
		static int SInterpolate(double xv, double& yv, double& yd,
			const std::vector<double>& xT, const std::vector<double>& yT);


		void toto(int r);

	private:

			static int SBinary_Search(double xv, const std::vector<double>& xT);

		};

	}
