#ifndef __IC_MODULES_H
#define __IC_MODULES_H

#include "basic.h"
#include "lpm.h"

#define IC_MODULE_INIT_WITH_NAME(name) ic_##name##_module_init
#define IC_MODULE_CLEANUP_WITH_NAME(name) ic_##name##_module_cleanup

typedef int (*ic_module_init)(void);
typedef int (*ic_module_cleanup)(void);

/* init all the modules */
void modules_init();

/* cleanup all the modules */
void modules_cleanup();

#endif
