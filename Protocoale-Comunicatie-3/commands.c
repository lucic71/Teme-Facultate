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
#include <json_parser.h>
#include <memory.h>
#include <http.h>

#include <server.h>

int op_register(int sockfd) {
    /*
     * Display the prompt for username and password and get them.
     *
     */
    auth_info_t *auth_info = get_auth_info();
    NONVOID_ERROR_HANDLER(auth_info == NULL, "[ERROR] Could not get auth_info",
            OPERATION_FAILED);

    /*
     * Generate the json payload for the POST request.
     *
     */
    cJSON *register_json = convert_auth_info_to_cJSON(auth_info, USERNAME, PASSWORD);

    /*
     * Convert the JSON object to char array.
     *
     */
    char *register_json_string = cJSON_Print(register_json);

    /*
     * Generate the POST request. If the post request computation fails then
     * an all the memory allocated previously will be FREEd and the function
     * will return a OPERATION_FAILED error code.
     *
     */
    char *post_request = compute_post_request(SERVER, REGISTER_URL, 
            JSON_CONTENT_TYPE, &register_json_string, 1, NULL, 0, NULL);

    if (post_request == NULL) {
        cJSON_Delete(register_json);
        delete_auth_info(auth_info);

        FREE(register_json_string);

        NONVOID_ERROR_HANDLER(post_request == NULL, "[ERROR] Could not build POST request",
            OPERATION_FAILED);
    }

    /*
     * Send the POST request to the server and print the response.
     * If the post request computation fails then an all the memory allocated 
     * previously will be FREEd and the function will return a OPERATION_FAILED error code.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, post_request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        cJSON_Delete(register_json);
        delete_auth_info(auth_info);

        FREE(register_json_string);
        FREE(post_request);

        NONVOID_ERROR_HANDLER(true, "[ERROR] Could not send request to server",
                OPERATION_FAILED);
    }

    /*
     * Receive a response. If #receive_from_server returns NULL it means an error occured
     * and the register operation was not successful.
     *
     */
    char *response = receive_from_server(sockfd);
    if (response == NULL || contains_status_code(response, BAD_REQUEST)) {
        cJSON_Delete(register_json);
        delete_auth_info(auth_info);

        FREE(register_json_string);
        FREE(post_request);

        /*
         * Decide what status code to return and what error code to print.
         *
         */
        if (response == NULL) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Could not receive a response from server",
                    OPERATION_CONNECTION_CLOSED);
        } else if (contains_status_code(response, BAD_REQUEST)) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Bad request",
                    OPERATION_FAILED);
        }

    }

    puts(response);

    /*
     * FREE the memory.
     *
     */
    delete_auth_info(auth_info);

    cJSON_Delete(register_json);
    FREE(register_json_string);

    FREE(post_request);
    FREE(response);

    return OPERATION_SUCCESSFUL;
}

int login(int sockfd, char **cookie) {
    NONVOID_ERROR_HANDLER(cookie == NULL, "[ERROR] Cookie argument must be"
            " a valid address", OPERATION_FAILED);

    /*
     * Display the prompt for username and password and get them.
     *
     */
    auth_info_t *auth_info = get_auth_info();
    NONVOID_ERROR_HANDLER(auth_info == NULL, "[ERROR] Could not get auth_info",
            OPERATION_FAILED);

    /*
     * Generate the json payload for the POST request.
     *
     */
    cJSON *register_json = convert_auth_info_to_cJSON(auth_info, USERNAME, PASSWORD);

    /*
     * Convert the JSON object to char array.
     *
     */
    char *register_json_string = cJSON_Print(register_json);

    /*
     * Generate the POST request. If the post request computation fails then
     * an all the memory allocated previously will be FREEd and the function
     * will return a OPERATION_FAILED error code.
     *
     */
    char *post_request = compute_post_request(SERVER, LOGIN_URL, 
            JSON_CONTENT_TYPE, &register_json_string, 1, NULL, 0, NULL);

    if (post_request == NULL) {
        cJSON_Delete(register_json);
        delete_auth_info(auth_info);

        FREE(register_json_string);

        NONVOID_ERROR_HANDLER(post_request == NULL, "[ERROR] Could not build POST request",
            OPERATION_FAILED);
    }

    /*
     * Send the POST request to the server and print the response.
     * If the post request computation fails then an all the memory allocated 
     * previously will be FREEd and the function will return a OPERATION_FAILED error code.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, post_request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        cJSON_Delete(register_json);
        delete_auth_info(auth_info);

        FREE(register_json_string);
        FREE(post_request);

        NONVOID_ERROR_HANDLER(true, "[ERROR] Could not send request to server",
                OPERATION_FAILED);
    }

    /*
     * Receive a response. If #receive_from_server returns NULL it means an error occured
     * and the login operation was not successful.
     *
     * Also if the response contains a BAD_REQUEST status code then the response is
     * not valid.
     *
     */
    char *response = receive_from_server(sockfd);
    if (response == NULL || contains_status_code(response, BAD_REQUEST)) {
        cJSON_Delete(register_json);
        delete_auth_info(auth_info);

        FREE(register_json_string);
        FREE(post_request);

        /*
         * Decide what status code to return and what error code to print.
         *
         */
        if (response == NULL) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Could not receive a response from server",
                    OPERATION_CONNECTION_CLOSED);
        } else if (contains_status_code(response, BAD_REQUEST)) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Bad request",
                    OPERATION_FAILED);
        }
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

    /*
     * FREE the memory.
     *
     */
    delete_auth_info(auth_info);

    cJSON_Delete(register_json);
    FREE(register_json_string);

    FREE(post_request);
    FREE(response);

    return OPERATION_SUCCESSFUL;
}

int enter_library(int sockfd, char *cookie, char **jwt_token) {

    /*
     * Put guards on @cookie and @jwt_token before beginning this operation.
     *
     */
    NONVOID_ERROR_HANDLER(cookie == NULL, "[ERROR] cookie must be a"
            " non-null address", OPERATION_FAILED);

    NONVOID_ERROR_HANDLER(jwt_token == NULL, "[ERROR] jwt_token must be a"
            " non-null address", OPERATION_FAILED);

    /*
     * Compute a get request using the @cookie.
     *
     */
    char *get_request = compute_get_request(SERVER, ENTER_LIBRARY_URL, NULL, &cookie, 1, NULL);
    NONVOID_ERROR_HANDLER(get_request == NULL, "[ERROR] Could not build GET request",
            OPERATION_FAILED);

    /*
     * Send the POST request to the server and print the response.
     * If the post request computation fails then an all the memory allocated 
     * previously will be FREEd and the function will return a OPERATION_FAILED error code.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, get_request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        FREE(get_request);
        NONVOID_ERROR_HANDLER(true, "[ERROR] Could not send request to server",
                OPERATION_FAILED);
    }

    /*
     * Receive a response. If #receive_from_server returns NULL it means an error occured
     * and the login operation was not successful.
     *
     * Also if the response contains a BAD_REQUEST status code then the response is
     * not valid.
     *
     */
    char *response = receive_from_server(sockfd);
    if (response == NULL || contains_status_code(response, BAD_REQUEST)) {
        FREE(get_request);

        /*
         * Decide what status code to return and what error code to print.
         *
         */
        if (response == NULL) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Could not receive a response from server",
                    OPERATION_CONNECTION_CLOSED);
        } else if (contains_status_code(response, BAD_REQUEST)) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Bad request",
                    OPERATION_FAILED);
        }
    }

    char *json_body = basic_extract_json_response(response);

    /*
     * Parse the JWT token received in the HTTP response.
     *
     */
    cJSON *json_received_object = cJSON_Parse(json_body);
    if (json_received_object == NULL) {
        FREE(get_request);
        FREE(response);

        NONVOID_ERROR_HANDLER(true, cJSON_GetErrorPtr(), OPERATION_FAILED);
    }

    cJSON *cJSON_token = cJSON_GetObjectItemCaseSensitive(json_received_object,
            JWT_TOKEN);

    /*
     * Put the token in @jwt_token.
     *
     */
    CALLOC(*jwt_token, strlen(cJSON_token->valuestring) + 1);
    strcpy(*jwt_token, cJSON_token->valuestring);

    /*
     * FREE the memory.
     *
     */
    cJSON_Delete(json_received_object);

    FREE(get_request);
    FREE(response);

    return OPERATION_SUCCESSFUL;
}

int get_books(int sockfd, char *jwt_token) {
    /*
     * Guard for input.
     *
     */
    NONVOID_ERROR_HANDLER(jwt_token == NULL, "[ERROR] jwt_token must be a"
            " non-null address", OPERATION_FAILED);

    /*
     * Add the jwt token to the GET request.
     *
     */
    char *get_request = compute_get_request(SERVER, GET_BOOKS_URL, NULL, NULL, 0, jwt_token);
    if (get_request == NULL) {
        NONVOID_ERROR_HANDLER(get_request == NULL, "[ERROR] Could not build POST request",
            OPERATION_FAILED);
    }

    /*
     * Send the POST request to the server and print the response.
     * If the post request computation fails then an all the memory allocated 
     * previously will be FREEd and the function will return a OPERATION_FAILED error code.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, get_request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        FREE(get_request);
        NONVOID_ERROR_HANDLER(true, "[ERROR] Could not send request to server",
                OPERATION_FAILED);
    }

    /*
     * Receive a response. If #receive_from_server returns NULL it means an error occured
     * and the login operation was not successful.
     *
     * Also if the response contains a BAD_REQUEST status code then the response is
     * not valid.
     *
     */
    char *response = receive_from_server(sockfd);
    if (response == NULL || contains_status_code(response, BAD_REQUEST)
            || contains_status_code(response, FORBIDDEN)) {
        FREE(get_request);

        /*
         * Decide what status code to return and what error code to print.
         *
         */
        if (response == NULL) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Could not receive a response from server",
                    OPERATION_CONNECTION_CLOSED);
        } else if (contains_status_code(response, BAD_REQUEST)) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Bad request",
                    OPERATION_FAILED);
        } else if (contains_status_code(response, FORBIDDEN)) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Forbidden",
                    OPERATION_FAILED);
        }
    }

    puts(basic_extract_json_list_response(response));

    /*
     * FREE the memory.
     *
     */
    FREE(get_request);
    FREE(response);

    return OPERATION_SUCCESSFUL;
}

int add_book(int sockfd, char *jwt_token) {
    /*
     * Guard for input.
     *
     */
    NONVOID_ERROR_HANDLER(jwt_token == NULL, "[ERROR] jwt_token must be a"
            " non-null address", OPERATION_FAILED);

    /*
     * Get book info.
     *
     */
    book_info_t *book_info = get_book_info();
    NONVOID_ERROR_HANDLER(book_info == NULL, "[ERROR] Could not get book_info",
            OPERATION_FAILED);

    /*
     * Generate the payload for the POST request.
     *
     */
    cJSON *book_json = book_to_json(book_info);
    char *book_json_string = cJSON_Print(book_json);

    /*
     * Generate the POST request.
     *
     */
    char *post_request = compute_post_request(SERVER, ADD_BOOK_URL,
        JSON_CONTENT_TYPE, &book_json_string, 1, NULL, 0, jwt_token);

    if (post_request == NULL) {
        cJSON_Delete(book_json);
        delete_book_info(book_info);

        FREE(book_json_string);

        NONVOID_ERROR_HANDLER(post_request == NULL, "[ERROR] Could not build POST request",
            OPERATION_FAILED);
    }

    /*
     * Send POST request.
     *
     */
    int send_to_server_ret = send_to_server(sockfd, post_request);
    if (send_to_server_ret == SEND_TO_SERVER_FAILED) {
        cJSON_Delete(book_json);
        delete_book_info(book_info);

        FREE(book_json_string);
        FREE(post_request);

        NONVOID_ERROR_HANDLER(true, "[ERROR] Could not send request to server",
                OPERATION_FAILED);
    }


    /*
     * Receive a response. If #receive_from_server returns NULL it means an error occured
     * and the login operation was not successful.
     *
     * Also if the response contains a BAD_REQUEST status code then the response is
     * not valid.
     *
     */
    char *response = receive_from_server(sockfd);
    if (response == NULL || contains_status_code(response, BAD_REQUEST)) {
        cJSON_Delete(book_json);
        delete_book_info(book_info);

        FREE(book_json_string);
        FREE(post_request);

        /*
         * Decide what status code to return and what error code to print.
         *
         */
        if (response == NULL) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Could not receive a response from server",
                    OPERATION_CONNECTION_CLOSED);
        } else if (contains_status_code(response, BAD_REQUEST)) {
            NONVOID_ERROR_HANDLER(true, "[ERROR] Bad request",
                    OPERATION_FAILED);
        }
    }

    puts("Book added successfully");

    return OPERATION_SUCCESSFUL;
}





