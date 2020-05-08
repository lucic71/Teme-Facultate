#include <stddef.h>
#include <string.h>

#include <stdio.h>

#include <http.h>
#include <cJSON.h>
#include "error.h"

#define ERROR_JSON_OBJECT "error"

bool contains_status_code(char *response, char *status_code) {

    if (strstr(response, status_code) != NULL) {
        return true;
    } else {
        return false;
    }

}

char *basic_extract_json_response(char *response) {
    return strstr(response, "{\"");
}

char *basic_extract_json_list_response(char *response) {
    return strstr(response, "[");
}

char *basic_extract_cookie_response(char *response) {
    return strstr(response, SET_COOKIE) + SET_COOKIE_SIZE;
}

char *basic_extract_crlf_response(char *response) {
    return strstr(response, CRLF);
}

void print_http_error(char *json_response) {

    if (json_response == NULL) {
        return;
    }

    cJSON *cjson_response = cJSON_Parse(json_response);

    if (cjson_response == NULL) {
        return;
    }

    cJSON *cjson_error = cJSON_GetObjectItemCaseSensitive(cjson_response, ERROR_JSON_OBJECT);

    ERROR(cjson_error->valuestring);
    cJSON_Delete(cjson_response);

}

