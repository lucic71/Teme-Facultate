#ifndef COMMANDS_H_
#define COMMANDS_H_

/*
 * Maximum sizes of different inputs.
 *
 */

#define MAX_COMMAND_SZ 20

/*
 * Format of commands given by the user.
 *
 */

#define REGISTER      "register"
#define LOGIN         "login"
#define ENTER_LIBRARY "enter_library"
#define GET_BOOKS     "get_books"
#define ADD_BOOK      "add_book"
#define EXIT          "exit"


/*
 * Status codes for all commands listed below.
 *
 */

#define OPERATION_SUCCESSFUL          0
#define OPERATION_FAILED              1
#define OPERATION_CONNECTION_CLOSED   2

/*
 * Registers an account to the server. The connection
 * with the server must be established on @sockfd.
 *
 * If the server closed the connection with the client then
 * OPERATION_CONNECTION_CLOSED error code will be returned. If any other
 * error occured then OPERATION_FAILED will be returned.
 *
 * On success, OPERATION_SUCCESSFUL is returned.
 *
 */

int op_register(int sockfd);

/*
 * Logins to the server and returns a session cookie.
 *
 * If this function fails then it will return NULL.
 *
 */

int login(int sockfd, char **cookie);

/*
 * Using a @cookie, send a GET request to the library access URL and return
 * a JWT Token in @jwt_token.
 *
 */
int enter_library(int sockfd, char *cookie, char **jwt_token);

/*
 * Using a @jwt_token, send a GET request to the library URL and print the
 * received list of JSON object.
 *
 */
int get_books(int sockfd, char *jwt_token);


/*
 * Using a @jwt_token, send a POST request to the add book URL. The info
 * about the book will be fed from a prompt.
 *
 */
int add_book(int sockfd, char *jwt_token);

#endif



