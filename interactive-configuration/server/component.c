#include "component.h"

/*
 * the last one means error happens
 * used to terminate the connection
 * */
ic_handler_func         msg_funcs[IC_CMD_MAX + 1];
ic_handler_func_init    msg_funcs_init[IC_CMD_MAX + 1];
ic_handler_func_cleanup    msg_funcs_cleanup[IC_CMD_MAX + 1];

/*
 * TODO: if you add new component, please add description below
 * */
static char *COMPONENT_DESCRIPTION[IC_CMD_MAX + 1] = {
    "Connection Setup Component",
    "Connection Termination Component",
    "LPM Filename Exchange Component",
    "LPM Data Transfer Component",
    "Close Connection Anyway!",
};

int IC_COMPONENT_REGISTER(struct ic_component *component) {

    int ret = -1;

    printf("\tRegister the [%s]\n", COMPONENT_DESCRIPTION[component->cmd_id]);

    if (!component || component->cmd_id > IC_CMD_MAX || !component->msg_handler \
            || !component->msg_handler_init || !component->msg_handler_cleanup) {
        
        if (component->cmd_id > IC_CMD_MAX)
            printf("\tERROR: Invalid component with ID %d\n", component->cmd_id);
        else
            printf("\tERROR: Cannot register the [%s]!\n", COMPONENT_DESCRIPTION[component->cmd_id]);

        return -1;
    }

    msg_funcs[component->cmd_id] = component->msg_handler;
    msg_funcs_init[component->cmd_id] = component->msg_handler_init;
    msg_funcs_cleanup[component->cmd_id] = component->msg_handler_cleanup;


    ret = msg_funcs_init[component->cmd_id]();

    if (ret < 0) {
        printf("\tERROR: Cannot initialize the [%s]!\n", COMPONENT_DESCRIPTION[component->cmd_id]);
        return -1;
    }

    return 0;
}

int IC_COMPONENT_ARRAY_REGISTER(struct ic_component component[], int ncomp) {
    int i = 0;
    int ret = -1;

    for (i = 0; i < ncomp; i++) {

        ret = IC_COMPONENT_REGISTER(&component[i]);

        if (ret < 0) {
            return -1;
        }
    }

    return 0;
}

int IC_COMPONENT_UNREGISTER(struct ic_component *component) {

    int ret = -1;

    printf("\tUnregister the [%s]\n", COMPONENT_DESCRIPTION[component->cmd_id]);

    if (!component || component->cmd_id > IC_CMD_MAX || !component->msg_handler \
            || !component->msg_handler_init || !component->msg_handler_cleanup) {
        
        if (component->cmd_id > IC_CMD_MAX)
            printf("\tERROR: Invalid component with ID %d\n", component->cmd_id);
        else
            printf("\tERROR: Cannot unregister the [%s]!\n", COMPONENT_DESCRIPTION[component->cmd_id]);

        return -1;
    }

    ret = msg_funcs_cleanup[component->cmd_id]();

    if (ret < 0) {
        printf("\tERROR: Cannot cleanup the [%s]!\n", COMPONENT_DESCRIPTION[component->cmd_id]);
        return -1;
    }

    return 0;
}

int IC_COMPONENT_ARRAY_UNREGISTER(struct ic_component component[], int ncomp) {
    int i = 0;
    int ret = -1;

    for (i = 0; i < ncomp; i++) {

        ret = IC_COMPONENT_UNREGISTER(&component[i]);

        if (ret < 0) {
            return -1;
        }
    }

    return 0;
}

/* some ic components may not need to do any init/cleanup work */
int default_icc_init_func() {
    return 0;
}

int default_icc_cleanup_func() {
    return 0;
}
