#include <stdlib.h>     
#include <stdio.h>
#include <unistd.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include <stdbool.h>

#include <connection.h>
#include <error.h>
#include <buffer.h>

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
    NONVOID_ERROR_HANDLER(sockfd < 0, "[ERROR] open_connection: socket", OPEN_CONNECTION_FAILED);

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
     * Connect to server.
     *
     */
    int connect_ret = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    NONVOID_ERROR_HANDLER(connect_ret < 0, "[ERROR] open_connection: socket", OPEN_CONNECTION_FAILED);

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
        NONVOID_ERROR_HANDLER(bytes < 0, "[ERROR] send_to_server: write ", SEND_TO_SERVER_FAILED);

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
            NONVOID_ERROR_HANDLER(true, "[ERROR] receive_from_server: read ", NULL);
        }

        if (bytes == 0) {
            buffer_destroy(&buffer);
            NONVOID_ERROR_HANDLER(true, "[DEBUG] The server closed the connection.", NULL);
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
            NONVOID_ERROR_HANDLER(true, "[ERROR] receive_from_server: read ", NULL);
        }

        if (bytes == 0) {
            buffer_destroy(&buffer);
            NONVOID_ERROR_HANDLER(true, "[DEBUG] The server closed the connection.", NULL);
        }

        buffer_add(&buffer, response, (size_t) bytes);
    }

    buffer_add(&buffer, "", 1);
    return buffer.data;
}







