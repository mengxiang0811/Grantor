#ifndef _HANDLER_H
#define _HANDLER_H

#include <time.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

enum COMMAND {
    FILE_NAME_CMD,
    LPM_DATA_CMD,
    CONN_TERM_CMD,
    CMD_MAX
};

#define CSP_OK      "200 OK: Ready\n"
#define CSP_ERROR   "404 ERROR: Invalid Connection Setup Message\n"
#define DTP_ERROR   "404 ERROR: Invalid Data Transfer Message\n"
#define CTP_OK      "200 OK: Closing Connection\n"
#define CTP_ERROR   "404 ERROR: Invalid Connection Termination Message\n"

#endif
