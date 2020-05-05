#ifndef ERROR_H_
#define ERROR_H_

/*
 * Success messages for the operations defined in commands.h
 *
 */
#define REGISTER_SUCCESS      "User registered successfully!"
#define LOGIN_SUCCESS         "User logged in successfully!"
#define ENTER_LIBRARY_SUCCESS "Entered library successfully!"
#define ADD_BOOK_SUCCESS      "Book added successfully!"
#define DELETE_BOOK_SUCCESS   "Book deleted successfully!"
#define LOGOUT_SUCCESS        "User logged out successfully!"

/*
 * Error messages for the operations defined in commands.h
 *
 */
#define REGISTER_FAIL      "Username already exists!"
#define LOGIN_FAIL         "User does not exist!"
#define ENTER_LIBRARY_FAIL "Entered library failed!"
#define ADD_BOOK_FAIL      "Book cannot be added at the moment!"
#define GET_BOOKS_FAIL     "Books cannot be received at the moment!"
#define GET_BOOK_FAIL      "Book cannot be received at the moment!"
#define DELETE_BOOK_FAIL   "Book cannot be deleted at the moment!"
#define LOGOUT_FAIL        "User cannot be logged out at the moment"

/*
 * Error message for inexistent operations.
 *
 */
#define INVALID_COMMAND_ERROR "This command is not supported!"

/*
 * Error messages for sending and receiving a packet.
 *
 */
#define SEND_ERROR "Could not send request to server"
#define RECV_ERROR "Could not receive a response from server"

/*
 * Error messages for generating requests.
 *
 */
#define POST_ERROR "Could not generate POST request"
#define GET_ERROR  "Could not generate GET request"
#define DELETE_ERROR  "Could not generate DELETE request"

/*
 * Error messages for prompters.
 *
 */
#define AUTH_INFO_ERROR "Could not get authentication info"
#define BOOK_INFO_ERROR "Could not get book info"

/*
 * Error messages for the books received as JSON objects.
 *
 */
#define NO_BOOKS_ERROR "There are no books in the library!"

/*
 * Error messages for connection with the server.
 *
 */
#define SERVER_CONNECTION_CLOSED "Server closed the connection. Please retry the operation!"
#define NO_INTERNET_CONNECTION   "There is no internet connection!"

/*
 * Error messages for invalid cookies or tokens.
 *
 */
#define ACTIVE_COOKIE_ERROR "You must logout before making this operation!"
#define COOKIE_ERROR        "You must login before making this operation!"
#define JWT_TOKEN_ERROR     "You must enter library before making this operation!"
#define ENTER_LIBARY_ERROR  "You are already in library!"

#define ERROR_HANDLER(assertion, return_value) \
	do {								\
		if (assertion) {					\
            return return_value; \
		}							\
	} while(0)

#define ERROR(error)    \
    do {    \
        fprintf(stderr, "[ERROR] %s\n", error);     \
    } while (0)

#endif
