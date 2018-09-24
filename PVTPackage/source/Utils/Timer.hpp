
#ifdef WIN32	// Windows implementation
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
class Timer
{
public:
	typedef long long Counter;
	static void init();

	static void tic(Counter & counter_)
	{
		QueryPerformanceCounter(&tmp);
		counter_ -= tmp.QuadPart;
	}

	static void toc(Counter & counter_)
	{
		QueryPerformanceCounter(&tmp);
		counter_ += tmp.QuadPart;
	}
	
	static void toctic(Counter & tocCounter_, Counter & ticCounter_)
	{
		QueryPerformanceCounter(&tmp);
		tocCounter_ += tmp.QuadPart;
		ticCounter_ -= tmp.QuadPart;
	}

	static double elapsed(Counter & counter_);

private:
	static double Freq;
	static LARGE_INTEGER tmp;
};
#else	// non-Windows implementation
#include "sys/time.h"
#include "sys/types.h"

class Timer
{
public:
	typedef int64_t Counter;
	static void init();

	static void tic(Counter & counter_)
	{
		gettimeofday(&tmp, nullptr);
		counter_ -= (tmp.tv_sec * 1000000 + tmp.tv_usec);
	}

	static void toc(Counter & counter_)
	{
		gettimeofday(&tmp, nullptr);
		counter_ += (tmp.tv_sec * 1000000 + tmp.tv_usec);
	}
	
	static void toctic(Counter & tocCounter_, Counter & ticCounter_)
	{
		gettimeofday(&tmp, nullptr);
		tocCounter_ += (tmp.tv_sec * 1000000 + tmp.tv_usec);
		ticCounter_ -= (tmp.tv_sec * 1000000 + tmp.tv_usec);
	}

	static double elapsed(Counter & counter_);

private:
	static struct timeval tmp;
};
#endif

