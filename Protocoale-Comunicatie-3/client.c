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

/*
 * This macro makes a stable connection with the server.
 *
 */
#define RESTORE_SOCKFD(assertion, sockfd, server_ip) \
    do {    \
        if (assertion) {    \
            ERROR(SERVER_CONNECTION_CLOSED);     \
            sockfd = -1;    \
            while (sockfd < 0) {    \
                sockfd = open_connection(server_ip, PORT, AF_INET, SOCK_STREAM, 0); \
            }   \
        }   \
    } while (sockfd < 0)

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

        if (strncmp(input_buffer, REGISTER, sizeof(REGISTER) - 1) == 0) {

            int register_ret = op_register(&sockfd);

            /*
             * If the connection closed during the register operation then a new
             * socket must be opened for the communication with the server.
             *
             */
            RESTORE_SOCKFD(register_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, LOGIN, sizeof(LOGIN) - 1) == 0) {
            
            int login_ret = login(&sockfd, &cookie);
            RESTORE_SOCKFD(login_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

            /*
             * If the login was successful then the client must delete the previous
             * @jwt_token.
             *
             */
            if (login_ret == OPERATION_SUCCESSFUL) {
                FREE(jwt_token);
            }

        } else if (strncmp(input_buffer, ENTER_LIBRARY, sizeof(ENTER_LIBRARY) - 1) == 0) {

            /*
             * If a jwt_token was already stored in the @jwt_token then
             * delete it because a new enter_library will generate a new jwt_token.
             *
             */
            int enter_library_ret = enter_library(&sockfd, cookie, &jwt_token);
            RESTORE_SOCKFD(enter_library_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, GET_BOOKS, sizeof(GET_BOOKS) - 1) == 0) {

            int get_books_ret = get_books(&sockfd, jwt_token);
            RESTORE_SOCKFD(get_books_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, GET_BOOK, sizeof(GET_BOOK) - 1) == 0) {

            int get_book_ret = get_book(&sockfd, jwt_token);
            RESTORE_SOCKFD(get_book_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, ADD_BOOK, sizeof(ADD_BOOK) - 1) == 0) {

            int add_book_ret = add_book(&sockfd, jwt_token);
            RESTORE_SOCKFD(add_book_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, DELETE_BOOK, sizeof(DELETE_BOOK) - 1) == 0) {

            int delete_book_ret = delete_book(&sockfd, jwt_token);
            RESTORE_SOCKFD(delete_book_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);
        
        } else if (strncmp(input_buffer, LOGOUT, sizeof(LOGOUT) - 1) == 0) {

            int logout_ret = logout(&sockfd, cookie);
            RESTORE_SOCKFD(logout_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

            /*
             * Delete the cookie and the jwt_token if the operation was successful.
             *
             */
            if (logout_ret == OPERATION_SUCCESSFUL) {
                FREE(cookie);
                FREE(jwt_token);
            }

        } else if (strncmp(input_buffer, EXIT, sizeof(EXIT) - 1) == 0) {

            puts("Closing the connection with the server, goodbye.");
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
