#include <stdlib.h>     
#include <stdio.h>
#include <unistd.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>

#include "connection.h"
#include "error.h"
#include "buffer.h"
#include "server.h"
#include "dns.h"

void compute_message(char *message, const char *line) {

    strcat(message, line);
    strcat(message, "\r\n");

}

int open_connection(char *host_ip, int portno, int ip_type, int socket_type, int flag) {
    /*
     * Open the socket.
     *
     */
    int sockfd = socket(ip_type, socket_type, flag);
    ERROR_HANDLER(sockfd < 0, OPEN_CONNECTION_FAILED);

    /*
     * Fill the information about the server in a struct sockaddr_in.
     *
     */
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0x00, sizeof(serv_addr));

    serv_addr.sin_family = ip_type;
    serv_addr.sin_port = htons(portno);
    inet_aton(host_ip, &serv_addr.sin_addr);

    /*
     * Set timeout.
     *
     */
    struct timeval tv;
    tv.tv_sec  = CONNECTION_TIMEOUT;
    tv.tv_usec = 0;

    setsockopt(sockfd, IPPROTO_TCP, SO_RCVTIMEO, (const char *) &tv, sizeof(tv));

    /*
     * Connect to server.
     *
     */
    int connect_ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    ERROR_HANDLER(connect_ret < 0, OPEN_CONNECTION_FAILED);

    return sockfd;
}

void close_connection(int sockfd) {

    close(sockfd);

}

int send_to_server(int sockfd, char *message) {
    int bytes, sent = 0;
    int total = strlen(message);

    do {
        bytes = write(sockfd, message + sent, total - sent);
        ERROR_HANDLER(bytes < 0, SEND_TO_SERVER_FAILED);

        if (bytes == 0) {
            puts("[DEBUG] Message sent successfully!");
            break;
        }

        sent += bytes;
    } while (sent < total);

    return SEND_TO_SERVER_SUCCESSFUL;
}

char *receive_from_server(int sockfd) {

    char response[BUFLEN];
    buffer buffer = buffer_init();
    int header_end = 0;
    int content_length = 0;

    /*
     * Read the header.
     *
     */
    do {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            buffer_destroy(&buffer);
            ERROR_HANDLER(true, NULL);
        }

        if (bytes == 0) {
            buffer_destroy(&buffer);
            ERROR_HANDLER(true, NULL);
        }

        buffer_add(&buffer, response, (size_t) bytes);
        
        header_end = buffer_find(&buffer, HEADER_TERMINATOR, HEADER_TERMINATOR_SIZE);

        if (header_end >= 0) {
            header_end += HEADER_TERMINATOR_SIZE;
            
            int content_length_start = buffer_find_insensitive(&buffer, CONTENT_LENGTH, CONTENT_LENGTH_SIZE);
            
            if (content_length_start < 0) {
                continue;           
            }

            content_length_start += CONTENT_LENGTH_SIZE;
            content_length = strtol(buffer.data + content_length_start, NULL, 10);
            break;
        }
    } while (1);

    /*
     * Read the content.
     *
     */
    size_t total = content_length + (size_t) header_end;
    
    while (buffer.size < total) {
        int bytes = read(sockfd, response, BUFLEN);

        if (bytes < 0){
            buffer_destroy(&buffer);
            ERROR_HANDLER(true, NULL);
        }

        if (bytes == 0) {
            buffer_destroy(&buffer);
            ERROR_HANDLER(true, NULL);
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }

    buffer_add(&buffer, "", 1);
    return buffer.data;
}


int send_and_receive(int *sockfd, char *request, char **response) {

    char *server_ip = get_ip(SERVER);

    /*
     * If the internet connection dies while the client runs, get_ip will
     * detect this because it makes a DNS request that will use the connection.
     *
     * So the client will die to.
     *
     */
    if (server_ip == NULL) {
        ERROR(NO_INTERNET_CONNECTION);
        exit(EXIT_FAILURE);
    }

    bool connection_failed = false;

    int return_status = SEND_RECV_FAIL;

    for (size_t i = 0; i < SEND_RECV_TIMEOUT; i++) {

        /*
         * Repoen the connection if it closed.
         *
         */
        if (connection_failed == true) {
            *sockfd = open_connection(server_ip, PORT, AF_INET, SOCK_STREAM, 0);
            connection_failed = false;
        }

        /*
         * Maybe the connection failed and sockfd is now -1.
         *
         */
        if (*sockfd == -1) {
            continue;
        }

        /*
         * Try to send to server and to receive.
         *
         */
        int send_to_server_ret = send_to_server(*sockfd, request);

        if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
            connection_failed = true;
            continue;
        }

        *response = receive_from_server(*sockfd);

        if (*response == NULL) {
            connection_failed = true;
            continue;
        }

        /*
         * If the above operations did not fail then break from loop.
         *
         */
        return_status = SEND_RECV_SUCC;
        break;
    }

    return return_status;

}

void restore_connection(int *sockfd, char *server_ip) {

    for (size_t i = 0; i < SEND_RECV_TIMEOUT; i++) {
        *sockfd = open_connection(server_ip, PORT, AF_INET, SOCK_STREAM, 0);

        if (*sockfd > 0) {
            return;
        }

    }

    ERROR(UNSTABLE_INTERNET_CONNECTION);
    exit(EXIT_FAILURE);

}
