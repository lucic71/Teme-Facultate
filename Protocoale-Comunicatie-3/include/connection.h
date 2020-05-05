#ifndef CONNECTION_H_
#define CONNECTION_H_

#define BUFLEN 4096
#define LINELEN 1000

#define HEADER_TERMINATOR "\r\n\r\n"
#define HEADER_TERMINATOR_SIZE (sizeof(HEADER_TERMINATOR) - 1)

#define CONTENT_LENGTH "Content-Length: "
#define CONTENT_LENGTH_SIZE (sizeof(CONTENT_LENGTH) - 1)

/*
 * Adds a line to a string message.
 *
 */
void compute_message(char *message, const char *line);

/*
 * Opens a connection with the server at @host_ip on port @portno.
 *
 * If the connection is successful then it returns a new socket file descriptor,
 * else it returns OPEN_CONNECTION_FAILED.
 *
 */
#define OPEN_CONNECTION_FAILED -1

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag);

/*
 * Closes the connection with the server on socket sockfd.
 *
 */
void close_connection(int sockfd);

/*
 * Sends a message to a server on socket sockfd.
 *
 * Returns SEND_TO_SERVER_SUCCESSFUL on success and
 * SEND_TO_SERVER_FAILED on failure.
 *
 */
#define SEND_TO_SERVER_SUCCESSFUL 0
#define SEND_TO_SERVER_FAILED     1

int send_to_server(int sockfd, char *message);

/*
 * Receives and returns a message from server.
 *
 */
char *receive_from_server(int sockfd);

/*
 * Sends request and tries SEND_RECV_TIMEOUT times to receive a response.
 *
 * Returns SEND_RECV_FAIL on failure and SEND_RECV_SUCC on success.
 *
 */
#define SEND_RECV_FAIL -1
#define SEND_RECV_SUCC 0

#define SEND_RECV_TIMEOUT 10

int send_and_receive(int *sockfd, char *request, char **response);

#endif
