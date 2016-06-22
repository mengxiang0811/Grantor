#ifndef __COMMON_H
#define __COMMON_H

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

typedef int (*handler_func_init)(void);
typedef int (*handler_func)( int );

/*
 * interactive configuration (IC)
 * */
enum IC_REQ_CMD {
    /* connection management commands */
    CONN_SETUP_CMD,
    CONN_TERM_CMD,

    /* LPM rules transfer commands */
    LPM_FNAME_CMD,
    LPM_DATA_CMD,

    /* add new request types */

    /* indicate the #CMDs */
    CMD_MAX
};

enum IC_RET_CMD {
    IC_OK,
    IC_ERROR,

    /* add new return status */

    IC_MAX,
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

static char *COMPONENT_NAME_MAPPING[CMD_MAX + 1] = {
    "Connection Setup Component",
    "Connection Termination Component",
    "LPM Filename Exchange Component",
    "LPM Data Transfer Component",
    "Close Connection Anyway!",
};

static char *IC_OK_MSG[CMD_MAX * 2] = {
    /* for control message transfer */
    "200 OK: Connection Established\n",
    "200 OK: Closing Connection\n",
    "200 OK: Valid LPM Rule Filename Controle Message\n",
    "200 OK: Valid LPM Rule DATA Transfer Control Message\n",
    
    /* for data transfer */
    "200 OK: Connection Established (Never Used!!!)\n",
    "200 OK: Closing Connection (Never Used!!!)\n",
    "200 OK: Valid LPM Rule Filename\n",
    "200 OK: Valid LPM Rule DATA\n",
};

static char *IC_ERROR_MSG[CMD_MAX + 2] = {
    "404 ERROR: Invalid Connection Setup Message\n",
    "404 ERROR: Invalid Connection Termination Message\n",
    "404 ERROR: Invalid LPM Rule Filename Message\n",
    "404 ERROR: Invalid LPM Rule DATA Transfer Message\n",
    "405 ERROR: Not Defined Command\n", //CMD_MAX
    "406 ERROR: Cannot Receive Message\n", //CMD_MAX + 1
};

void common_init();

int receive_ctrl_msg(int sockfd, int expected_cmd);
int receive_data_msg(int sockfd, int data_len);

int connection_setup_init();
int connection_setup(int sockfd);

int connection_terminate_init();
int connection_terminate(int sockfd);

int connection_terminate_directly_init();
int connection_terminate_directly(int sockfd);

#endif
