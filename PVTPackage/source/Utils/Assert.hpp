#pragma once

#include "Logger.hpp"

namespace PVTPackage {
#ifndef NDEBUG
#define ASSERT(condition, message) do { if (!(condition)) { LOGERROR(message); } } while(false)
#else
#define ASSERT(condition, message) do { (void)sizeof(condition); } while(false)
#endif
}
