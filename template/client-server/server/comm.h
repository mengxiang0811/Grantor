#ifndef __IC_COMMON_H
#define __IC_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RCV_SUCC    0
#define RCV_ERROR   1
#define RCV_DISCON  2

#define BUFF_MAX 1024
#define PAYLOAD_MAX (1024 * 1024)

/*
 * interactive configuration (IC)
 * registered modules
 * */
enum IC_MODULES_ID {
    IC_BASIC_MODULE,
    IC_LPM_MODULE,
    IC_MODULE_MAX,
};

/*
 * IC request commands
 * */
enum IC_REQ_CMD {
    /* connection management commands */
    IC_CONN_SETUP_CMD,
    IC_CONN_TERM_CMD,

    /* LPM rules transfer commands */
    IC_LPM_FNAME_CMD,
    IC_LPM_DATA_CMD,

    /* add new request types */

    /* indicate the #CMDs */
    IC_CMD_MAX,
};

/*
 * IC request return status
 * */
enum IC_RET_CMD {
    IC_OK,
    IC_ERROR,

    /* add new return status */

    IC_RET_MAX,
};

struct ic_request {
    uint32_t cmd_id;
    uint32_t length;
    char     *data;
};

struct ic_reply {
    uint32_t status;
    uint32_t length;
    char     *info;
};

void common_init();

int receive_ctrl_msg(int sockfd, int expected_cmd);
int receive_data_msg(int sockfd, int data_len);

#endif
