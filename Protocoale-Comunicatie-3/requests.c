#include <stdlib.h>     
#include <stdio.h>
#include <unistd.h>     
#include <string.h>     
#include <stdbool.h>

#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>

#include "connection.h"
#include "requests.h"
#include "error.h"
#include "memory.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char *auth_token) {
    char *message;
    CALLOC(message, BUFLEN);

    char *line;
    CALLOC(line, LINELEN);

    /*
     * Append queries to the first line of the request.
     *
     */
    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    if (host == NULL) {
           
        FREE(message);
        FREE(line);

        ERROR_HANDLER(true, NULL);
    }

    memset(line, 0x00, LINELEN);
    sprintf(line, "Host: %s", host);

    compute_message(message, line);

    /*
     * Add headers and/or cookies, according to the protocol format
     *
     */
    if (cookies != NULL) {

        memset(line, 0x00, LINELEN);
        sprintf(line, "Cookie: %s", cookies[0]);

        for (size_t i = 1; i < cookies_count; i++) {
            sprintf(line, "%s; %s", line, cookies[i]);
        }

        compute_message(message, line);
       
    }

    /*
     * Add the authorization token.
     *
     */
    if (auth_token != NULL) {
        memset(line, 0x00, LINELEN);
        sprintf(line, "Authorization: Bearer %s", auth_token);

        compute_message(message, line);
    }

    /*
     * Add a final newline.
     *
     */
    compute_message(message, "");

    /*
     * FREE the memory and return.
     *
     */
    FREE(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count, char *auth_token) {
    char *message;
    CALLOC(message, BUFLEN);

    char *line;
    CALLOC(line, LINELEN);

    /*
     * Append queries to the first line of the request.
     *
     */
    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    if (host == NULL) {
           
        FREE(message);
        FREE(line);

        ERROR_HANDLER(true, NULL);
    }

    memset(line, 0x00, LINELEN);
    sprintf(line, "Host: %s", host);

    compute_message(message, line);

    /*
     * Add headers and/or cookies, according to the protocol format
     *
     */
    if (cookies != NULL) {

        memset(line, 0x00, LINELEN);
        sprintf(line, "Cookie: %s", cookies[0]);

        for (size_t i = 1; i < cookies_count; i++) {
            sprintf(line, "%s; %s", line, cookies[i]);
        }

        compute_message(message, line);
       
    }

    /*
     * Add the authorization token.
     *
     */
    if (auth_token != NULL) {
        memset(line, 0x00, LINELEN);
        sprintf(line, "Authorization: Bearer %s", auth_token);

        compute_message(message, line);
    }

    /*
     * Add a final newline.
     *
     */
    compute_message(message, "");

    /*
     * FREE the memory and return.
     *
     */
    FREE(line);
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
        int body_data_fields_count, char **cookies, int cookies_count, char *auth_token) {

    char *message;
    CALLOC(message, BUFLEN);

    char *line;
    CALLOC(line, LINELEN);

    char *body_data_buffer;
    CALLOC(body_data_buffer, LINELEN);

    /*
     * Write the method name, URL and protocol type.
     *
     */
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    /*
     * Add the host.
     *
     */
    if (host == NULL) {

        FREE(message);
        FREE(line);
        FREE(body_data_buffer);

        ERROR_HANDLER(true, NULL);
    }

    memset(line, 0x00, LINELEN);
    sprintf(line, "Host: %s", host);

    compute_message(message, line);

    /*
     * Add Content-Type to the request.
     *
     */
    memset(line, 0x00, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);

    compute_message(message, line);


    if (body_data_fields_count == 0) {
        FREE(message);
        FREE(line);
        FREE(body_data_buffer);

        ERROR_HANDLER(true, NULL);
    }

    /*
     * Compute Content-Length.
     *
     */
    size_t content_length = 0;
    for (size_t i = 0; i < body_data_fields_count; i++) {
        content_length += strlen(body_data[i]);
    }

    // add the number of &'s from body_Data to length
    content_length += (body_data_fields_count - 1);

    memset(line, 0x00, LINELEN);
    sprintf(line, "Content-Length: %ld", content_length);

    compute_message(message, line);

    /*
     * Add the cookies.
     *
     */
    if (cookies != NULL) {
       
        memset(line, 0x00, LINELEN);
        sprintf(line, "Cookie: %s", cookies[0]);

        for (size_t i = 1; i < cookies_count; i++) {
            sprintf(line, "%s; %s", line, cookies[i]);
        }

        compute_message(message, line);

    }
    /*
     * Add the authorization token.
     *
     */
    if (auth_token != NULL) {
        memset(line, 0x00, LINELEN);
        sprintf(line, "Authorization: Bearer %s", auth_token);

        compute_message(message, line);
    }

    /*
     * Add a newline at the final of the header.
     */
    compute_message(message, "");

    /*
     * Add the payload of the POST request.
     *
     */
    memset(body_data_buffer, 0x00, LINELEN);
    sprintf(body_data_buffer, "%s", body_data[0]);

    for (size_t i = 1; i < body_data_fields_count; i++) {
        sprintf(body_data_buffer, "%s&%s", body_data_buffer, body_data[i]);
    }

    compute_message(message, body_data_buffer);

    /*
     * FREE the memory and return.
     *
     */
    FREE(line);
    FREE(body_data_buffer);

    return message;
}
