#ifndef DEBUGHEADER_H
#define DEBUGHEADER_H

//#define NDEBUG

#include <cassert>

#ifdef NDEBUG
#define D(x)
#else
#define D(x) x
#endif

#endif // DEBUGHEADER_H
