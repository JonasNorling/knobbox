#pragma once

#ifdef HOST

#include <cstdio>
#define LOG(x...) ::fprintf(stderr, x)

#else

#define LOG(x...)

#endif
