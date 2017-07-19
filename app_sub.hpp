#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"

#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)

#endif
