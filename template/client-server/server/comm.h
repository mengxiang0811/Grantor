#ifndef __COMMON_H
#define __COMMON_H

#include <time.h> 
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

#endif
