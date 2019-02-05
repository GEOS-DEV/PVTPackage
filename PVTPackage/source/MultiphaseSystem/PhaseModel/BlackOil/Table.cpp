/* 
 * File: Table.cpp
 * ---------------
 * Implementation for Table Class
 * Copied from GPRS
 */
#include "Table.hpp"    				
#include <cassert>


namespace PVTPackage
{

	using namespace std;

	int Table::SBinary_Search(double xv, const std::vector<double>& xT)
	{
		int npnts = xT.size();
		int min = 0, max = npnts - 1, mid;
		bool incr = (xT[0] < xT[npnts - 1]);    // xT is increasing or decreasing

		while (true) {
			if (max - min == 1) break;
			mid = (max + min) / 2;          // split into two parts

																			// --- find which part xv belongs to ---
			if (incr == 1) {						// xT increasing			
				(xT[mid] <= xv) ? min = mid : max = mid;
				mid += 0;
				mid += 0;
			}
			else {										// xT decreasing
				(xT[mid] >= xv) ? min = mid : max = mid;
			}
		}

		return min;
	}

	int Table::SInterpolate(double xv, double& yv, double& yd,
		const std::vector<double>& xT, const std::vector<double>& yT)
	{
		int npnts = xT.size();
		// --- xv is out of the range of xT[] ---
		if (xT[0] < xT[npnts - 1]) {			// increaing xT
			if (xv <= xT[0]) { yv = yT[0]; yd = 0.0; return 0; }
			if (xv >= xT[npnts - 1]) { yv = yT[npnts - 1]; yd = 0.0; return npnts - 2; }
		}
		else {										// decreasing xT
			if (xv >= xT[0]) { yv = yT[0]; yd = 0.0; return 0; }
			if (xv <= xT[npnts - 1]) { yv = yT[npnts - 1]; yd = 0.0; return npnts - 2; }
		}

		// --- xv is within the range of xT[] ---
		int indx = SBinary_Search(xv, xT);						// find the location of xv in xT[]

		yd = (yT[indx + 1] - yT[indx]) / (xT[indx + 1] - xT[indx]);	// linear interperlation
		yv = yT[indx] + yd*(xv - xT[indx]);

		return indx;
	}

	
}  //end PVTPackage



