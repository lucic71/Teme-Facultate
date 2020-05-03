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
#define REGISTER_FAIL      "User cannot be registered at the moment!"
#define LOGIN_FAIL         "User cannot be logged in at the moment!"
#define ENTER_LIBRARY_FAIL "Entered library failed!"
#define ADD_BOOK_FAIL      "Book cannot be added at the moment!"
#define GET_BOOKS_FAIL     "Books cannot be received at the moment!"
#define GET_BOOK_FAIL      "Book cannot be received at the moment!"
#define DELETE_BOOK_FAIL   "Book cannot be deleted at the moment!"
#define LOGOUT_FAIL        "User cannot be logged out at the moment"

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
 * Error messages for HTTP received packets.
 *
 */
#define BAD_REQUEST_ERROR "Bad request!"
#define NOT_FOUND_ERROR   "Not found!"
#define FORBIDDEN_ERROR   "Forbidden! Try to login or enter_library first."

/*
 * Error messages for prompters.
 *
 */
#define AUTH_INFO_ERROR "Could not get authentication info"
#define BOOK_INFO_ERROR "Could not get book info"

/*
 * Error messages for invalid cookies or tokens.
 *
 */
#define ACTIVE_COOKIE_ERROR "There is an active cookie in this session, you must logout!"
#define COOKIE_ERROR        "Cookie must be a non-null address!"
#define JWT_TOKEN_ERROR     "JWT token must be a non-null address!"

#define VOID_ERROR_HANDLER(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
            return; \
		}							\
	} while(0)

#define NONVOID_ERROR_HANDLER(assertion, call_description, return_value) \
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
            return return_value; \
		}							\
	} while(0)

#define ERROR(error)    \
    do {    \
        fprintf(stderr, "[ERROR] %s\n", error);     \
    } while (0)

#endif
