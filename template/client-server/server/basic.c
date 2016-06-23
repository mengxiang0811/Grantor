#include "basic.h"

static struct ic_component icc_basic[] = {
    {IC_CONN_SETUP_CMD, connection_setup, default_icc_init_func, default_icc_cleanup_func},
    {IC_CONN_TERM_CMD, connection_terminate, default_icc_init_func, default_icc_cleanup_func},
    {IC_CMD_MAX, connection_terminate_directly, default_icc_init_func, default_icc_cleanup_func},
};

int ic_basic_module_init() {
    return IC_COMPONENT_ARRAY_REGISTER(icc_basic, IC_COMPONENT_NUM(basic));
}

int ic_basic_module_cleanup() {
    return IC_COMPONENT_ARRAY_UNREGISTER(icc_basic, IC_COMPONENT_NUM(basic));
}

int connection_setup(int sockfd) {
    return receive_ctrl_msg(sockfd, IC_CONN_SETUP_CMD);
}

int connection_terminate(int sockfd) {
    return connection_terminate_directly(sockfd);
    //return receive_ctrl_msg(sockfd, CONN_TERM_CMD);
}

int connection_terminate_directly(int sockfd) {
    printf("Close the connection directly!\n");
    close(sockfd);
    sleep(1);

    return 0;
}
