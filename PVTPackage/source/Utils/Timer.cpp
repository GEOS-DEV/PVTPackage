/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "Timer.hpp"

#ifdef WIN32	// Windows implementation
void Timer::init()
{
	QueryPerformanceFrequency(&tmp);
	Freq = static_cast<double>(tmp.QuadPart);
}

double Timer::elapsed(Counter & counter_)
{
	return counter_/Freq;
}

double Timer::Freq;
LARGE_INTEGER Timer::tmp;

#else	// non-Windows implementation
void Timer::init()
{
}

double Timer::elapsed(Counter & counter_)
{
  return counter_ * 1.e-6;
}

struct timeval Timer::tmp;

#endif
