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

/* the status of receiving message */
#define RCV_SUCC    0
#define RCV_ERROR   1
#define RCV_DISCON  2

/* the size of the message buffer */
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

/* the format of the message */
struct ic_request {
    uint32_t cmd_id;
    uint32_t length;
    char     *data;
};

/* the format of the reply message */
struct ic_reply {
    uint32_t status;
    uint32_t length;
    char     *info;
};

/* the init & cleanup functions */
int common_init();
int common_cleanup();

/* 
 * generic function to receive any control message
 *
 * @sockfd: the socket descriptor
 * @expected_cmd: the expected REQUEST ID that the server would receive
 *                  -1 means any REQUEST ID is fine
 * */
int receive_ctrl_msg(int sockfd, int expected_cmd);

/*
 * generic function to receive any message data
 *
 * @sockfd: the socket descriptor
 * @data_len: the length of the receiving data
 * */
int receive_data_msg(int sockfd, int data_len);

#endif
