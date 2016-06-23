#ifndef __IC_COMPONENT_H
#define __IC_COMPONENT_H

#include "comm.h"

typedef int (*ic_handler_func_init)(void);
typedef int (*ic_handler_func_cleanup)(void);
typedef int (*ic_handler_func)(int);

struct ic_component {
    enum IC_REQ_CMD         cmd_id;

    ic_handler_func         msg_handler;
    ic_handler_func_init    msg_handler_init;
    ic_handler_func_cleanup    msg_handler_cleanup;
};

#define IC_COMPONENT_NUM(module_name) (sizeof(icc_##module_name) / sizeof(struct ic_component))

int IC_COMPONENT_REGISTER(struct ic_component *component);

int IC_COMPONENT_ARRAY_REGISTER(struct ic_component component[], int ncomp);

/*
 * TODO: release resources when the component is unregistered
 * */
int IC_COMPONENT_UNREGISTER(struct ic_component *component);
int IC_COMPONENT_ARRAY_UNREGISTER(struct ic_component component[], int ncomp);

int default_icc_init_func();
int default_icc_cleanup_func();

#endif
