#include "basic.h"

static struct ic_component icc_basic[] = {
    {IC_CONN_SETUP_CMD, connection_setup, connection_setup_init},
    {IC_CONN_TERM_CMD, connection_terminate, connection_terminate_init},
    {IC_CMD_MAX, connection_terminate_directly, connection_terminate_directly_init},
};

int ic_basic_module_init() {
    return IC_COMPONENT_ARRAY_REGISTER(icc_basic, IC_COMPONENT_NUM(basic));
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

int connection_setup_init() {
    return 0;
}

int connection_terminate_init() {
    return 0;
}

int connection_terminate_directly_init() {
    return 0;
}
