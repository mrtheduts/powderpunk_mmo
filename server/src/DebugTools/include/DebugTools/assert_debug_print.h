#ifndef ASSERT_DEBUG_PRINT_H
#define ASSERT_DEBUG_PRINT_H

#ifdef NDEBUG
// Assert values only when built in Debug mode
#include <assert.h>
#endif

// Print only when compiled in debug mode
#ifdef NDEBUG
#define DEBUG(x)
#else
#include <iostream>
#define DEBUG(x) do {std::cerr << x << std::endl;} while(0)
#endif

#endif
