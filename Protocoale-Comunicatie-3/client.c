#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <error.h>
#include <connection.h>
#include <server.h>
#include <dns.h>
#include <memory.h>
#include <commands.h>

/*
 * TODO: do not forget to test the return value of sockfd.
 *
 */
#define RESTORE_SOCKFD(assertion, sockfd, server_ip) \
    do {    \
        if (assertion) {    \
            puts("[DEBUG] The server closed the connection, opening a new socket"); \
            sockfd = open_connection(server_ip, PORT, AF_INET, SOCK_STREAM, 0); \
            printf("[DEBUG] Socket is now: %d\n", sockfd);  \
        }   \
    } while (0)


int main() {

    /*
     * Get the IP of the server.
     *
     */
    char *server_ip = get_ip(SERVER);
    printf("[DEBUG] Server IPv4 is: %s\n", server_ip);

    /*
     * Open a connection with the server.
     *
     */
    int sockfd = open_connection(server_ip, PORT, AF_INET, SOCK_STREAM, 0);
    printf("[DEBUG] Connection established on socket: %d\n", sockfd);

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

            int register_ret = op_register(sockfd);

            /*
             * If the connection closed during the register operation then a new
             * socket must be opened for the communication with the server.
             *
             */
            RESTORE_SOCKFD(register_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, LOGIN, sizeof(LOGIN) - 1) == 0) {
            
            int login_ret = login(sockfd, &cookie);
            RESTORE_SOCKFD(login_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

            printf("The cookie is: %s\n", cookie);

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
            FREE(jwt_token);

            int enter_library_ret = enter_library(sockfd, cookie, &jwt_token);
            RESTORE_SOCKFD(enter_library_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

            printf("The token is: %s\n", jwt_token);

        } else if (strncmp(input_buffer, GET_BOOKS, sizeof(GET_BOOKS) - 1) == 0) {

            int get_books_ret = get_books(sockfd, jwt_token);
            RESTORE_SOCKFD(get_books_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, GET_BOOK, sizeof(GET_BOOK) - 1) == 0) {

            int get_book_ret = get_book(sockfd, jwt_token);
            RESTORE_SOCKFD(get_book_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, ADD_BOOK, sizeof(ADD_BOOK) - 1) == 0) {

            int add_book_ret = add_book(sockfd, jwt_token);
            RESTORE_SOCKFD(add_book_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

        } else if (strncmp(input_buffer, DELETE_BOOK, sizeof(DELETE_BOOK) - 1) == 0) {

            int delete_book_ret = delete_book(sockfd, jwt_token);
            RESTORE_SOCKFD(delete_book_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);
        
        } else if (strncmp(input_buffer, LOGOUT, sizeof(LOGOUT) - 1) == 0) {

            int logout_ret = logout(sockfd, cookie);
            RESTORE_SOCKFD(logout_ret == OPERATION_CONNECTION_CLOSED, 
                    sockfd, server_ip);

            // delete the cookie if the operation was successful
            if (logout_ret == OPERATION_SUCCESSFUL) {
                FREE(cookie);
            }

        } else if (strncmp(input_buffer, EXIT, sizeof(EXIT) - 1) == 0) {

            puts("[DEBUG] Closing the connection with the server, goodbye.");
            close(sockfd);

            FREE(cookie);
            FREE(jwt_token);

            break;

        }

    }

    return EXIT_SUCCESS;
}
