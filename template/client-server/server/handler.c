#include "handler.h"

handler_func msg_funcs[CMD_MAX + 1];
handler_func_init msg_funcs_init[CMD_MAX + 1];

void handler_init() {

    common_init();

    /* init the functions */
    msg_funcs[CONN_SETUP_CMD] = connection_setup;
    msg_funcs[CONN_TERM_CMD] = connection_terminate;
    msg_funcs[LPM_FNAME_CMD] = get_lpm_rule_fname;
    msg_funcs[LPM_DATA_CMD] = get_lpm_rule_data;
    msg_funcs[CMD_MAX] = connection_terminate_directly;
    
    /* init the component initialization functions */
    msg_funcs_init[CONN_SETUP_CMD] = connection_setup_init;
    msg_funcs_init[CONN_TERM_CMD] = connection_terminate_init;
    msg_funcs_init[LPM_FNAME_CMD] = get_lpm_rule_fname_init;
    msg_funcs_init[LPM_DATA_CMD] = get_lpm_rule_data_init;
    msg_funcs_init[CMD_MAX] = connection_terminate_directly_init;
    
    int i = 0;
    int ret = -1;

    for (i = 0; i <= CMD_MAX; i++) {

        printf("\nInitialize the [%s]\n", COMPONENT_NAME_MAPPING[i]);

        ret = msg_funcs_init[i]();

        if (ret < 0) {
            printf("ERROR: Cannot initialize the [%s]!\n", COMPONENT_NAME_MAPPING[i]);
            exit(-1);
        }
    }
}
