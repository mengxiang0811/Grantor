#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

#include "handler.h"

extern struct ic_request    req;
extern handler_func msg_funcs[CMD_MAX + 1];

void Close_connection(int sockfd) {
    printf("close the connect!\n");
    close(sockfd);
    sleep(1);
}

int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    unsigned short port = 58000;
    int recvSize = -1;

    struct sockaddr_in serv_addr; 

    int ret = -1;

    if (argc != 2) {
        printf("\n Usage: %s <port#>\n",argv[0]);
        return 1;
    }

    port = atoi(argv[1]);

    /* init the handler functions & data structures*/
    handler_init();

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 20);

    while (1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        int ret = -1;

        printf("Accept a new client with socket %d\n", connfd);
        ret = connection_setup(connfd);

        if (ret < 0) 
            goto CONN_TERM_DIRECT;

        printf("Connection established!\n");

        while (1) {
            ret = receive_ctrl_msg(connfd, -1);

            printf("receive_ctrl_msg ret = %d\n", ret);

            if (ret < 0)
                break;

            ret = msg_funcs[req.cmd_id](connfd);
            
            printf("msg_funcs[%d] ret = %d\n", req.cmd_id, ret);

            if (ret < 0)
                break;

            if (req.cmd_id == CONN_TERM_CMD)
                break;
        }

CONN_TERM_DIRECT:
        connection_terminate_directly(connfd);
    }
}
