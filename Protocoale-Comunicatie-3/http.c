#include <stddef.h>
#include <string.h>

#include <http.h>

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

char *basic_extract_cookie_response(char *response) {
    return strstr(response, SET_COOKIE) + SET_COOKIE_SIZE;
}

char *basic_extract_crlf_response(char *response) {
    return strstr(response, CRLF);
}
