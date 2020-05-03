#ifndef HTTP_H_
#define HTTP_H_

#include <stdbool.h>

/*
 * Name of a JWT Token in then JSON object received in #enter_library.
 *
 */
#define JWT_TOKEN "token"

/*
 * Status codes received in HTTP responses.
 *
 */
#define SUCCESS "200 OK"
#define CREATED "201 Created"

/*
 * Function that checks if HTTP @response contains @status_code.
 *
 */
bool contains_status_code(char *response, char *status_code);

/*
 * Functions that extract data from a HTTP response.
 *
 */

#define CRLF "\r\n"
#define CRLF_SIZE (sizeof(CRLF) - 1)

#define SET_COOKIE "Set-Cookie: "
#define SET_COOKIE_SIZE (sizeof(SET_COOKIE) - 1)

char *basic_extract_json_response(char *response);

char *basic_extract_json_list_response(char *response);

char *basic_extract_cookie_response(char *respose);

char *basic_extract_crlf_response(char *response);


#endif
