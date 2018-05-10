#pragma once
#ifndef _MACROSUTILS
#define _MACROSUTILS


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                        MACRO UTILS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PP_NARG(...) EXPAND( PP_NARG_(__VA_ARGS__, PP_RSEQ_N()) )
#define PP_NARG_(...) EXPAND( PP_ARG_N(__VA_ARGS__) )
#define PP_ARG_N(_1, _2, _3, N, ...) N
#define PP_RSEQ_N() 3, 2, 1, 0
#define EXPAND(X) X
#define NAME(X) #X


#endif