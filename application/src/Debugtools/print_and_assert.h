// Assert values only when built in Debug mode
#include <assert.h>

// Print only when compiled in debug mode
#ifdef NDEBUG
#define DEBUG(x)
#else
#define DEBUG(x) do {std::cerr << x << std::endl;} while(0)
#endif

