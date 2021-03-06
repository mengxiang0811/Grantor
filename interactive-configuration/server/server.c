#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>

#include "modules.h"

/* 
 * The server process will listen to a UNIX socket, and handle connections.
 * It could accept only one active connection to avoid dealing with concurrency.
 *
 * To process a message, the server will first receive a control message from 
 * the client, which tells the server the meaning of the following data.
 *
 * The format of a typical message is as follows:
 *      the command id  (control message)
 *      the length of the message data  (control message)
 *
 *      the message data    (the real data to process)
 * */


extern struct ic_request    req;
extern ic_handler_func msg_funcs[IC_CMD_MAX + 1];

int listenfd = 0;
/* TODO: add signal handler */
static volatile int exiting = 0;

    static void
signal_handler(int signum)
{
    if (signum == SIGINT)
        fprintf(stderr, "caught SIGINT\n");
    else if (signum == SIGTERM)
        fprintf(stderr, "caught SIGTERM\n");
    else
        fprintf(stderr, "caught unknown signal\n");

    exiting = 1;

    close(listenfd);

    modules_cleanup();
}

int main(int argc, char *argv[]) {
    int connfd = 0;
    unsigned short port = 58000;
    int recvSize = -1;

    struct sockaddr_in serv_addr; 

    int ret = -1;

    if (argc != 2) {
        printf("\n Usage: %s <port#>\n",argv[0]);
        return 1;
    }

    port = atoi(argv[1]);

    /* init the handler functions & data structures of all the modules */
    modules_init();

    /* init the server socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 20);

    /* add support for signal handling */
    struct sigaction new_action;
    new_action.sa_handler = signal_handler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction(SIGINT, &new_action, NULL);
    sigaction(SIGTERM, &new_action, NULL);

    while (!exiting) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        if (connfd < 0)
            continue;

        int ret = -1;

        /* setup the connection */
        printf("Accept a new client with socket %d\n", connfd);
        ret = connection_setup(connfd);

        if (ret < 0) 
            goto CONN_TERM_DIRECT;

        printf("Connection established!\n");

        while (1) {
            /* to process a message, first, receive the control message */
            ret = receive_ctrl_msg(connfd, -1);

            printf("receive_ctrl_msg ret = %d\n", ret);

            if (ret < 0)
                break;

            if (req.cmd_id == IC_CONN_TERM_CMD)
                goto CONN_TERM_DIRECT;

            /* if the control message is valid for further process,
             * then process the data by the corresponding component
             * */
            ret = msg_funcs[req.cmd_id](connfd);

            printf("msg_funcs[%d] ret = %d\n", req.cmd_id, ret);

            if (ret < 0)
                break;
        }

        /* close the connection */
CONN_TERM_DIRECT:
        connection_terminate_directly(connfd);
    }

    printf("\nExsiting the server program!\n");

    return 0;
}
