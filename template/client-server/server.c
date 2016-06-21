#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define BUFF_MAX 1024
#define PAYLOAD_MAX 65536

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

void Close_connection(int sockfd) {
    close(sockfd);
    sleep(1);
}

int main(int argc, char *argv[]) {
    int listenfd = 0, connfd = 0;
    unsigned short port = 58000;
    int recvSize = -1;
    
    struct sockaddr_in serv_addr; 

    int ret = -1;

    uint32_t cmd_id;
    uint32_t data_len;
    char resp_msg[BUFF_MAX + PAYLOAD_MAX], message[BUFF_MAX + PAYLOAD_MAX], data[BUFF_MAX + PAYLOAD_MAX];

    if (argc != 2) {
        printf("\n Usage: %s <port#>\n",argv[0]);
        return 1;
    }

    port = atoi(argv[1]);

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(message, 0, sizeof(message)); 
    memset(resp_msg, 0, sizeof(resp_msg)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 20); 

    while(1) {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        /*
         *  Connection Setup Phase (CSP)
         *
         * */

        printf("Connection setup wait message!\n");
        memset(message, 0, sizeof(message));
        memset(resp_msg, 0, sizeof(resp_msg)); 
        memset(data, 0, sizeof(data)); 

        while ((recvSize = recv(connfd, resp_msg, (BUFF_MAX + PAYLOAD_MAX), 0)) > 0 && resp_msg[recvSize - 1] != '\n') {
            strncat(message, resp_msg, recvSize);
            memset(resp_msg, 0, sizeof(resp_msg));
        }

        if (recvSize < 0) {
            perror("CSP receive: ");

            if (send(connfd, CSP_ERROR, strlen(CSP_ERROR), 0) < 0) {
                perror("CSP send error: ");
            }

            Close_connection(connfd);
            continue;
        }

        strncat(message, resp_msg, recvSize);

        ret = sscanf(message, "%u %u %s\n", &cmd_id, &data_len, data);

        printf("Receive message: cmd = %u; data length = %u; filename = %s (%lu)\n", cmd_id, data_len, data, strlen(data));

        if (ret != 3 || cmd_id >= CMD_MAX || data_len <= 0 || data_len >= PAYLOAD_MAX) {
            if (send(connfd, CSP_ERROR, strlen(CSP_ERROR), 0) < 0) {
                perror("CSP - receive format error: ");
                Close_connection(connfd);
            }
        } else {

            /* 
             *
             * Return OK message, and proceed to transfer data
             *
             * */

            if (send(connfd, CSP_OK, strlen(CSP_OK), 0) < 0) {
                perror("CSP - reply: ");
                Close_connection(connfd);
                continue;
            }

            /*
             * Data Transfer Phase (DTP) 
             *
             * */

            //printf("In DTP!\n");
            while ((recvSize = recv(connfd, resp_msg, (BUFF_MAX + PAYLOAD_MAX), 0)) > 0 && data_len > 0) {
                strncat(message, resp_msg, recvSize);
                memset(resp_msg, 0, sizeof(resp_msg));
                data_len -= recvSize;
            }

            if (recvSize < 0) {
                perror("DTP receive: ");

                if (send(connfd, DTP_ERROR, strlen(DTP_ERROR), 0) < 0) {
                    perror("CSP send error: ");
                }

                Close_connection(connfd);
                continue;
            }

            strncat(message, resp_msg, strlen(resp_msg));
            memset(data, 0, sizeof(data)); 

            ret = sscanf(message, "%u %u %s\n", &cmd_id, &data_len, data);

            if (ret != 3 || cmd_id >= CMD_MAX || data_len <= 0 || data_len >= PAYLOAD_MAX) {
                if (send(connfd, DTP_ERROR, strlen(DTP_ERROR), 0) < 0) {
                    perror("CSP - receive format error: ");
                    Close_connection(connfd);
                    break;
                }
            }

            if (send(connfd, message, strlen(message), 0) < 0) {
                perror("DTP - reply: ");
                Close_connection(connfd);
                continue;
            }

            /*
             *  Connection Termination Phase (CTP)
             *
             * */

            memset(message, 0, sizeof(message));
            memset(resp_msg, 0, sizeof(resp_msg));

            while ((recvSize = recv(connfd, resp_msg, (BUFF_MAX + PAYLOAD_MAX), 0)) > 0 && resp_msg[recvSize - 1] != '\n') {
                strncat(message, resp_msg, recvSize);
                memset(resp_msg, 0, sizeof(resp_msg));
            }

            if (recvSize < 0) {
                perror("CTP receive: ");

                if (send(connfd, CTP_ERROR, strlen(CTP_ERROR), 0) < 0) {
                    perror("CTP send: ");
                }

                Close_connection(connfd);
                continue;
            }

            strncat(message, resp_msg, strlen(resp_msg));
            
            ret = sscanf(message, "%u\n", &cmd_id);

            if (ret != 1 || cmd_id != CONN_TERM_CMD) {
                if (send(connfd, CTP_ERROR, strlen(CTP_ERROR), 0) < 0) {
                    perror("CTP - receive error format: ");
                    Close_connection(connfd);
                }
            } else {
                if (send(connfd, CTP_OK, strlen(CTP_OK), 0) < 0) {
                    perror("CTP - reply: ");
                    Close_connection(connfd);
                }
            }
        }
    }
}
