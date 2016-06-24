#include "comm.h"

char *message;
static char msg_buff[BUFF_MAX];

struct ic_request    req;
static struct ic_reply      *rep;

struct ic_reply IC_OK_REPLY[IC_CMD_MAX * 2];
struct ic_reply IC_ERROR_REPLY[IC_CMD_MAX + 2];

/* 
 * define the interactive message,
 * if you need to add new modules and components,
 * then you need to add the corresponding messages here
 * */
static char *IC_OK_MSG[IC_CMD_MAX * 2] = {
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

static char *IC_ERROR_MSG[IC_CMD_MAX + 2] = {
    "404 ERROR: Invalid Connection Setup Message\n",
    "404 ERROR: Invalid Connection Termination Message\n",
    "404 ERROR: Invalid LPM Rule Filename Message\n",
    "404 ERROR: Invalid LPM Rule DATA Transfer Message\n",
    "405 ERROR: Not Defined Command\n", //IC_CMD_MAX
    "406 ERROR: Cannot Receive Message\n", //IC_CMD_MAX + 1
};

int common_init() {
    int i = 0;

    for (i = 0; i < IC_CMD_MAX * 2; i++) {
        IC_OK_REPLY[i].status = IC_OK;
        IC_OK_REPLY[i].length = strlen(IC_OK_MSG[i]);
        IC_OK_REPLY[i].info   = IC_OK_MSG[i];
    }

    for (i = 0; i < IC_CMD_MAX + 2; i++) {
        IC_ERROR_REPLY[i].status = IC_ERROR;
        IC_ERROR_REPLY[i].length = strlen(IC_ERROR_MSG[i]);
        IC_ERROR_REPLY[i].info   = IC_ERROR_MSG[i];
    }

    /* init the data structures */
    memset(msg_buff, 0, sizeof(msg_buff));
    message = (char *)calloc((BUFF_MAX + PAYLOAD_MAX), sizeof(char));

    memset(&req, 0, sizeof(struct ic_request));
    req.data = (char *)calloc((BUFF_MAX + PAYLOAD_MAX), sizeof(char));

    if (!message || !req.data) {
        printf("ERROR: allocate memory for message buffer!\n");
        return -1;
    }

    return 0;
}

int common_cleanup() {
    if (message) free(message);
    if (req.data) free(req.data);

    return 0;
}

static int rcv_process(int sockfd, int recv_size) {
    if (recv_size < 0) {
        perror("Receive Error: ");
#if 0
        memset(msg_buff, 0, sizeof(msg_buff));
        sprintf(msg_buff, "%u %u %s", IC_ERROR_REPLY[cmd].status, IC_ERROR_REPLY[cmd].length, IC_ERROR_REPLY[cmd].info);
        
        if (send(sockfd, msg_buff, BUFF_MAX, 0) < 0) {
#endif   
        if (send(sockfd, IC_ERROR_REPLY[IC_CMD_MAX + 1].info, IC_ERROR_REPLY[IC_CMD_MAX + 1].length, 0) < 0) {
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

    if (ret != 2 || req.cmd_id >= IC_CMD_MAX || (expected_cmd != -1 && req.cmd_id != expected_cmd) || req.length >= PAYLOAD_MAX) {
        int error_id = IC_CMD_MAX + 1;

        if (req.cmd_id <= IC_CMD_MAX)
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

    if (req.cmd_id == IC_CONN_TERM_CMD) {
        printf("Client is asking for terminating the connection!\n");
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

    printf("receive_data_msg send message = %s\n", IC_OK_REPLY[req.cmd_id + IC_CMD_MAX].info);
    if (send(sockfd, IC_OK_REPLY[req.cmd_id + IC_CMD_MAX].info, IC_OK_REPLY[req.cmd_id + IC_CMD_MAX].length, 0) < 0) {
        perror("Send: ");
        return -1;
    }

    return 0;
}
