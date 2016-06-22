#include "comm.h"

char msg_buff[BUFF_MAX];
char *message;

struct ic_request    req;
static struct ic_reply      *rep;

struct ic_reply IC_OK_REPLY[CMD_MAX * 2];
struct ic_reply IC_ERROR_REPLY[CMD_MAX + 2];

static int rcv_process(int sockfd, int recv_size) {
    if (recv_size < 0) {
        perror("Receive Error: ");
#if 0
        memset(msg_buff, 0, sizeof(msg_buff));
        sprintf(msg_buff, "%u %u %s", IC_ERROR_REPLY[cmd].status, IC_ERROR_REPLY[cmd].length, IC_ERROR_REPLY[cmd].info);
        
        if (send(sockfd, msg_buff, BUFF_MAX, 0) < 0) {
#endif   
        if (send(sockfd, IC_ERROR_REPLY[CMD_MAX + 1].info, IC_ERROR_REPLY[CMD_MAX + 1].length, 0) < 0) {
            perror("Send Error: ");
        }

        return RCV_ERROR;
    } else if (recv_size == 0) {
        printf("Client disconnected!\n");
        return RCV_DISCON;
    }

    return RCV_SUCC;
}

/*
 * @expected_cmd: -1 means any command is ok
 * */
int receive_ctrl_msg(int sockfd, int expected_cmd) {
    int ret = -1;
    int recv_size = -1;

    req.cmd_id = 0;
    req.length = 0;
    memset(req.data, 0, sizeof(char) * (BUFF_MAX + PAYLOAD_MAX));

    memset(msg_buff, 0, sizeof(msg_buff));
    memset(message, 0, sizeof(char) * (BUFF_MAX + PAYLOAD_MAX));

    printf("recive control %d informatoin!\n", expected_cmd);

    /* receive the control information:
     * i.e., the command id and the length of the data part
     * */
    while ((recv_size = recv(sockfd, msg_buff, BUFF_MAX, 0)) > 0 && msg_buff[recv_size - 1] != '\n') {
        strncat(message, msg_buff, recv_size);
        memset(msg_buff, 0, sizeof(msg_buff));
    }

    printf("Receive control information of size %d\n", recv_size);

    ret = rcv_process(sockfd, recv_size);

    if (ret != RCV_SUCC)
        return -1;

    strncat(message, msg_buff, recv_size);
    
    int j = 0;
    for (j = 0; j < recv_size; j++)
        printf("%c", msg_buff[j]);
    printf("\n");

    ret = sscanf(message, "%u %u\n", &req.cmd_id, &req.length);

    printf("Receive message: cmd = %u; data length = %u\n", req.cmd_id, req.length);

    if (ret != 2 || req.cmd_id >= CMD_MAX || (expected_cmd != -1 && req.cmd_id != expected_cmd) || req.length >= PAYLOAD_MAX) {
        int error_id = CMD_MAX + 1;

        if (req.cmd_id <= CMD_MAX)
            error_id = req.cmd_id;

        if (expected_cmd != -1)
            error_id = expected_cmd;

        if (send(sockfd, IC_ERROR_REPLY[error_id].info, IC_ERROR_REPLY[error_id].length, 0) < 0) {
            perror("Send: ");
        }

        return -1;
    }

    /* valid control information, reply OK to the client */
    if (send(sockfd, IC_OK_REPLY[req.cmd_id].info, IC_OK_REPLY[req.cmd_id].length, 0) < 0) {
        perror("Send: ");
        return -1;
    }

    return 0;
}

int receive_data_msg(int sockfd, int data_len) {
    /* receive the data with specific length */
    int ret = -1;
    int recv_size = -1;
    int file_size = data_len;

    printf("Receiving %d bytes of data!\n", file_size);

    if (file_size == 0)
        return 0;

    memset(msg_buff, 0, sizeof(msg_buff));
    memset(message, 0, sizeof(char) * (BUFF_MAX + PAYLOAD_MAX));

    while (file_size > 0 && (recv_size = recv(sockfd, msg_buff, BUFF_MAX, 0)) > 0) {
        strncat(message, msg_buff, recv_size);
        memset(msg_buff, 0, sizeof(msg_buff));

        file_size -= recv_size;
    }

    printf("recv_size = %d; file_size = %d; file content = %s\n", recv_size, file_size, message);

    ret = rcv_process(sockfd, recv_size);

    if (ret != RCV_SUCC)
        return -1;

    if (file_size != 0)
        return -1;

    printf("receive_data_msg send message = %s\n", IC_OK_REPLY[req.cmd_id + CMD_MAX].info);
    if (send(sockfd, IC_OK_REPLY[req.cmd_id + CMD_MAX].info, IC_OK_REPLY[req.cmd_id + CMD_MAX].length, 0) < 0) {
        perror("Send: ");
        return -1;
    }

    return 0;
}

int connection_setup(int sockfd) {
    return receive_ctrl_msg(sockfd, CONN_SETUP_CMD);
}

int connection_terminate(int sockfd) {
    return receive_ctrl_msg(sockfd, CONN_TERM_CMD);
}

int connection_terminate_directly(int sockfd) {
    printf("Close the connection directly!\n");
    close(sockfd);
    sleep(1);

    return 0;
}

int connection_setup_init() {
    return 0;
}

int connection_terminate_init() {
    return 0;
}

int connection_terminate_directly_init() {
    return 0;
}

void common_init() {
    int i = 0;

    for (i = 0; i < CMD_MAX * 2; i++) {
        IC_OK_REPLY[i].status = IC_OK;
        IC_OK_REPLY[i].length = strlen(IC_OK_MSG[i]);
        IC_OK_REPLY[i].info   = IC_OK_MSG[i];
    }

    for (i = 0; i < CMD_MAX + 2; i++) {
        IC_ERROR_REPLY[i].status = IC_ERROR;
        IC_ERROR_REPLY[i].length = strlen(IC_ERROR_MSG[i]);
        IC_ERROR_REPLY[i].info   = IC_ERROR_MSG[i];
    }

    /* init the data structures */
    memset(msg_buff, 0, sizeof(msg_buff));
    message = (char *)calloc((BUFF_MAX + PAYLOAD_MAX), sizeof(char));

    memset(&req, 0, sizeof(struct ic_request));
    req.data = (char *)calloc((BUFF_MAX + PAYLOAD_MAX), sizeof(char));
}
