#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "error.h"
#include "connection.h"
#include "server.h"
#include "dns.h"
#include "memory.h"
#include "commands.h"

int main() {

    /*
     * Get the IP of the server.
     *
     */
    char *server_ip = get_ip(SERVER);

    /*
     * If the DNS request did not succeed then there is no internet connection.
     *
     */
    if (server_ip == NULL) {
        ERROR(NO_INTERNET_CONNECTION);
        exit(EXIT_FAILURE);
    }

    /*
     * Open a connection with the server.
     *
     */
    int sockfd = open_connection(server_ip, PORT, AF_INET, SOCK_STREAM, 0);

    /*
     * Buffer for reading the input from the user.
     *
     */
    char input_buffer[MAX_COMMAND_SZ] = {0};

    /*
     * Login @cookie is computed in #login and @jwt_token is computed
     * in #enter_libary.
     *
     */
    char *cookie    = NULL;
    char *jwt_token = NULL;

    while (1) {
        /*
         * Display promt.
         *
         */
        printf("$ ");
        fflush(stdout);

        /*
         * Fill the buffer with 0's and read the input.
         *
         */
        memset(input_buffer, 0x00, MAX_COMMAND_SZ);
        fgets(input_buffer, MAX_COMMAND_SZ - 1, stdin);

        /*
         * Possible values:
         *
         * OPERATION_SUCCESSFUL
         * OPERATION_FAILED
         * OPERATION_CONNECTION_CLOSED
         *
         */
        int operation_status;

        if (strncmp(input_buffer, REGISTER, sizeof(REGISTER) - 1) == 0) {

            operation_status = op_register(&sockfd);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

        } else if (strncmp(input_buffer, LOGIN, sizeof(LOGIN) - 1) == 0) {
            
            operation_status = login(&sockfd, &cookie);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

            /*
             * If the login was successful then the client must delete the previous
             * @jwt_token.
             *
             */
            if (operation_status == OPERATION_SUCCESSFUL) {
                FREE(jwt_token);
            }

        } else if (strncmp(input_buffer, ENTER_LIBRARY, sizeof(ENTER_LIBRARY) - 1) == 0) {

            /*
             * If a jwt_token was already stored in the @jwt_token then
             * delete it because a new enter_library will generate a new jwt_token.
             *
             */
            operation_status = enter_library(&sockfd, cookie, &jwt_token);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

        } else if (strncmp(input_buffer, GET_BOOKS, sizeof(GET_BOOKS) - 1) == 0) {

            operation_status = get_books(&sockfd, jwt_token);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

        } else if (strncmp(input_buffer, GET_BOOK, sizeof(GET_BOOK) - 1) == 0) {

            operation_status = get_book(&sockfd, jwt_token);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

        } else if (strncmp(input_buffer, ADD_BOOK, sizeof(ADD_BOOK) - 1) == 0) {

            operation_status = add_book(&sockfd, jwt_token);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

        } else if (strncmp(input_buffer, DELETE_BOOK, sizeof(DELETE_BOOK) - 1) == 0) {

            operation_status = delete_book(&sockfd, jwt_token);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }
        
        } else if (strncmp(input_buffer, LOGOUT, sizeof(LOGOUT) - 1) == 0) {

            operation_status = logout(&sockfd, cookie);

            if (operation_status == OPERATION_CONNECTION_CLOSED) {
                restore_connection(&sockfd, server_ip);
            }

            /*
             * Delete the cookie and the jwt_token if the operation was successful.
             *
             */
            if (operation_status == OPERATION_SUCCESSFUL) {
                FREE(cookie);
                FREE(jwt_token);
            }

        } else if (strncmp(input_buffer, EXIT, sizeof(EXIT) - 1) == 0) {

            puts(GOODBYE);
            close(sockfd);

            FREE(cookie);
            FREE(jwt_token);

            break;

        } else {
            
            /*
             * The command does not exist.
             *
             */
            ERROR(INVALID_COMMAND_ERROR);

        }

    }

    return EXIT_SUCCESS;
}
