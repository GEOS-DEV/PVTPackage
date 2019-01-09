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
