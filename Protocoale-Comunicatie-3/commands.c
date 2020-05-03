#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <commands.h>
#include <cJSON.h>
#include <requests.h>
#include <error.h>
#include <connection.h>
#include <authentication.h>
#include <book.h>
#include <memory.h>
#include <http.h>

#include <server.h>

int op_register(int sockfd) {
    auth_info_t *auth_info     = NULL;
    cJSON *register_json       = NULL;
    char *register_json_string = NULL;
    char *request              = NULL;
    char *response             = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Get the authentication info.
     *
     */
    auth_info = get_auth_info();
    if (auth_info == NULL) {
        ERROR(AUTH_INFO_ERROR);

        return_status = OPERATION_FAILED;
        goto op_register_end;
    }

    /*
     * Generate the json payload for the POST request. And convert
     * the JSON object to a string.
     *
     */
    register_json        = auth_to_json(auth_info);
    register_json_string = cJSON_Print(register_json);

    /*
     * Generate the POST request.
     *
     */
    request = compute_post_request(SERVER, REGISTER_URL, 
            JSON_CONTENT_TYPE, &register_json_string, 1, NULL, 0, NULL);

    if (request == NULL) {
        ERROR(POST_ERROR);

        return_status = OPERATION_FAILED;
        goto op_register_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto op_register_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto op_register_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(REGISTER_FAIL);

        return_status = OPERATION_FAILED;
        goto op_register_end;

    }

    puts(REGISTER_SUCCESS);

op_register_end:

    delete_auth_info(auth_info);
    cJSON_Delete(register_json);

    FREE(register_json_string);
    FREE(request);
    FREE(response);

    return return_status;
}

int login(int sockfd, char **cookie) {

    auth_info_t *auth_info         = NULL;
    cJSON       *login_json        = NULL;
    char        *login_json_string = NULL;
    char        *request           = NULL;
    char        *response          = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (cookie == NULL) {
        ERROR(COOKIE_ERROR);

        return_status = OPERATION_FAILED;
        goto login_end;
    }

    /*
     * If there is already an active session then the user must
     * firstly logout before it wants to login.
     *
     */
    if (*cookie != NULL) {
        ERROR(ACTIVE_COOKIE_ERROR);

        return_status = OPERATION_FAILED;
        goto login_end;
    }

    /*
     * Get auth info.
     *
     */
    auth_info = get_auth_info();
    if (auth_info == NULL) {
        ERROR(AUTH_INFO_ERROR);

        return_status = OPERATION_FAILED;
        goto login_end;
    }


    /*
     * Generate the json payload for the POST request. And convert
     * the JSON object to a string.
     *
     */
    login_json        = auth_to_json(auth_info);
    login_json_string = cJSON_Print(login_json);

    /*
     * Generate the POST request.
     *
     */
    request = compute_post_request(SERVER, LOGIN_URL, 
            JSON_CONTENT_TYPE, &login_json_string, 1, NULL, 0, NULL);

    if (request == NULL) {
        ERROR(POST_ERROR);

        return_status = OPERATION_FAILED;
        goto login_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto login_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto login_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(LOGIN_FAIL);

        return_status = OPERATION_FAILED;
        goto login_end;

    }

    /*
     * Extract the cookie from the response and put its value in the @cookie argument.
     *
     */
    char *cookie_line     = basic_extract_cookie_response(response);
    char *cookie_line_end = basic_extract_crlf_response(cookie_line);

    cookie_line[cookie_line_end - cookie_line] = 0x00;

    CALLOC(*cookie, strlen(cookie_line) + 1);
    strcpy(*cookie, cookie_line);

    puts(LOGIN_SUCCESS);

login_end:

    delete_auth_info(auth_info);
    cJSON_Delete(login_json);

    FREE(login_json_string);
    FREE(request);
    FREE(response);

    return return_status;
}

int enter_library(int sockfd, char *cookie, char **jwt_token) {

    char *request  = NULL;
    char *response = NULL;

    cJSON *json_received_object = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (cookie == NULL) {
        ERROR(COOKIE_ERROR);

        return_status = OPERATION_FAILED;
        goto enter_library_end;
    }

    if (jwt_token == NULL) {
        ERROR(JWT_TOKEN_ERROR);

        return_status = OPERATION_FAILED;
        goto enter_library_end;
    }

    /*
     * Generate the GET request using the @cookie.
     *
     */
    request = compute_get_request(SERVER, ENTER_LIBRARY_URL, NULL, &cookie, 1, NULL);
    if (request == NULL) {
        ERROR(GET_ERROR);

        return_status = OPERATION_FAILED;
        goto enter_library_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto enter_library_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto enter_library_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(ENTER_LIBRARY_FAIL);

        return_status = OPERATION_FAILED;
        goto enter_library_end;

    }

    /*
     * Parse the JWT token received in the HTTP response.
     *
     */
    char *json_body = basic_extract_json_response(response);

    json_received_object = cJSON_Parse(json_body);

    cJSON *cJSON_token = cJSON_GetObjectItemCaseSensitive(json_received_object,
            JWT_TOKEN);

    /*
     * Put the token in @jwt_token.
     *
     */
    CALLOC(*jwt_token, strlen(cJSON_token->valuestring) + 1);
    strcpy(*jwt_token, cJSON_token->valuestring);

    puts(ENTER_LIBRARY_SUCCESS);

enter_library_end:

    cJSON_Delete(json_received_object);

    FREE(request);
    FREE(response);

    return return_status;
}

int get_books(int sockfd, char *jwt_token) {
    char *request  = NULL;
    char *response = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (jwt_token == NULL) {
        ERROR(JWT_TOKEN_ERROR);

        return_status = OPERATION_FAILED;
        goto get_books_end;
    }

    /*
     * Add the jwt token to the GET request.
     *
     */
    request = compute_get_request(SERVER, GET_BOOKS_URL, NULL, NULL, 0, jwt_token);
    if (request == NULL) {
        ERROR(GET_ERROR);

        return_status = OPERATION_FAILED;
        goto get_books_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto get_books_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto get_books_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(GET_BOOKS_FAIL);

        return_status = OPERATION_FAILED;
        goto get_books_end;

    }

    print_json_book_list(basic_extract_json_list_response(response));

get_books_end:

    FREE(request);
    FREE(response);

    return return_status;
}

int get_book(int sockfd, char *jwt_token) {

    book_id_t *book_id = NULL;
    char *book_url     = NULL;

    char *request  = NULL;
    char *response = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (jwt_token == NULL) {
        ERROR(JWT_TOKEN_ERROR);

        return_status = OPERATION_FAILED;
        goto get_book_end;
    }
    
    /*
     * Get the ID from user.
     *
     */
    book_id = get_book_id();

    /*
     * Add the jwt token to the GET request. Add the book id to URL.
     *
     */
    CALLOC(book_url, sizeof(GET_BOOKS_URL) - 1 + strlen("/") + strlen(book_id->id) + 1);
    sprintf(book_url, "%s/%s", GET_BOOKS_URL, book_id->id);

    request = compute_get_request(SERVER, book_url, NULL, NULL, 0, jwt_token);
    if (request == NULL) {
        ERROR(GET_ERROR);

        return_status = OPERATION_FAILED;
        goto get_book_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto get_book_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto get_book_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(GET_BOOK_FAIL);

        return_status = OPERATION_FAILED;
        goto get_book_end;

    }

    print_json_book(basic_extract_json_list_response(response));

get_book_end:

    delete_book_id(book_id);
    FREE(book_url);

    FREE(request);
    FREE(response);

    return return_status;

}


int add_book(int sockfd, char *jwt_token) {

    book_info_t *book_info        = NULL;
    cJSON       *book_json        = NULL;
    char        *book_json_string = NULL;
    char        *request          = NULL;
    char        *response         = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (jwt_token == NULL) {
        ERROR(JWT_TOKEN_ERROR);

        return_status = OPERATION_FAILED;
        goto add_book_end;
    }

    /*
     * Get book info.
     *
     */
    book_info = get_book_info();
    if (book_info == NULL) {
        ERROR("Could not get book info");

        return_status = OPERATION_FAILED;
        goto add_book_end;
    }

    /*
     * Generate the payload for the POST request.
     *
     */
    book_json = book_to_json(book_info);
    book_json_string = cJSON_Print(book_json);

    /*
     * Generate the POST request.
     *
     */
    request = compute_post_request(SERVER, ADD_BOOK_URL,
        JSON_CONTENT_TYPE, &book_json_string, 1, NULL, 0, jwt_token);

    if (request == NULL) {
        ERROR(POST_ERROR);

        return_status = OPERATION_FAILED;
        goto add_book_end;
    }

    /*
     * Send POST request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_FAILED;
        goto add_book_end;
    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto add_book_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(ADD_BOOK_FAIL);

        return_status = OPERATION_FAILED;
        goto add_book_end;
    }

    puts(ADD_BOOK_SUCCESS);

add_book_end:

    delete_book_info(book_info);
    cJSON_Delete(book_json);

    FREE(book_json_string);
    FREE(request);
    FREE(response);

    return return_status;

}

int delete_book(int sockfd, char *jwt_token) {

    book_id_t *book_id = NULL;
    char     *book_url = NULL;

    char *request  = NULL;
    char *response = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (jwt_token == NULL) {
        ERROR(JWT_TOKEN_ERROR);

        return_status = OPERATION_FAILED;
        goto delete_book_end;
    }
    
    /*
     * Get the ID from user.
     *
     */
    book_id = get_book_id();

    /*
     * Add the jwt token to the DELETE request. Add the book id to URL.
     *
     */
    CALLOC(book_url, sizeof(DELETE_BOOK_URL) - 1 + strlen("/") + strlen(book_id->id) + 1);
    sprintf(book_url, "%s/%s", DELETE_BOOK_URL, book_id->id);

    request = compute_delete_request(SERVER, book_url, NULL, NULL, 0, jwt_token);
    if (request == NULL) {
        ERROR(DELETE_ERROR);

        return_status = OPERATION_FAILED;
        goto delete_book_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto delete_book_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto delete_book_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(DELETE_BOOK_FAIL);

        return_status = OPERATION_FAILED;
        goto delete_book_end;

    }

    puts(DELETE_BOOK_SUCCESS);

delete_book_end:

    delete_book_id(book_id);
    FREE(book_url);

    FREE(request);
    FREE(response);

    return return_status;

}


int logout(int sockfd, char *cookie) {

    char *request  = NULL;
    char *response = NULL;

    int return_status = OPERATION_SUCCESSFUL;

    /*
     * Guard for input.
     *
     */
    if (cookie == NULL) {
        ERROR(COOKIE_ERROR);

        return_status = OPERATION_FAILED;
        goto logout_end;
    }

    /*
     * Generate the GET request using the @cookie.
     *
     */
    request = compute_get_request(SERVER, LOGOUT_URL, NULL, &cookie, 1, NULL);
    if (request == NULL) {
        ERROR(GET_ERROR);

        return_status = OPERATION_FAILED;
        goto logout_end;
    }

    /*
     * Send the request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        ERROR(SEND_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto logout_end;

    }

    /*
     * Receive the response.
     *
     */
    response = receive_from_server(sockfd);
    if (response == NULL) {
        ERROR(RECV_ERROR);

        return_status = OPERATION_CONNECTION_CLOSED;
        goto logout_end;
    }

    if (!contains_status_code(response, SUCCESS)) {
        ERROR(LOGOUT_FAIL);

        return_status = OPERATION_FAILED;
        goto logout_end;

    }

    puts(LOGOUT_SUCCESS);

logout_end:

    FREE(response);
    FREE(request);

    return return_status;

}















