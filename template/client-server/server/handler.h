#ifndef __HANDLER_H
#define __HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "comm.h"

#define RCV_SUCC    0
#define RCV_ERROR   1
#define RCV_DISCON  2

#define BUFF_MAX 1024
#define PAYLOAD_MAX (1024 * 1024)

typedef int (*handler_func)( int );

int receive_ctrl_msg(int sockfd, int expected_cmd);
int receive_data_msg(int sockfd, int data_len);

int connection_setup(int sockfd);
int connection_terminate(int sockfd);
int connection_terminate_directly(int sockfd);

int get_lpm_rule_fname(int sockfd);
int get_lpm_rule_data(int sockfd);

void handler_init();

#endif
