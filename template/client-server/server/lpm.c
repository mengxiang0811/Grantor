#include "lpm.h"

extern struct ic_request    req;

int get_lpm_rule_fname(int sockfd) {
    return receive_data_msg(sockfd, req.length);
}

int get_lpm_rule_data(int sockfd) {
    /* TODO: store the data to file*/
    return receive_data_msg(sockfd, req.length);
}
