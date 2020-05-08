#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#include "authentication.h"
#include "cJSON.h"
#include "error.h"
#include "memory.h"
#include "prompt.h"

auth_info_t *get_auth_info() {
    char *username = read_item(USERNAME, MAX_USER_SZ, ECHO_ON);
    char *password = read_item(PASSWORD, MAX_PASS_SZ, ECHO_OFF);

    if (username == NULL || password == NULL) {
        FREE(username);
        FREE(password);

        return NULL;
    }

    /*
     * Declare the return variable.
     *
     */
    auth_info_t *auth_info;
    CALLOC(auth_info, sizeof(auth_info_t));

    auth_info->username = username;
    auth_info->password = password;

    return auth_info;

}

cJSON *auth_to_json(auth_info_t *auth_info) {

    cJSON *json_object = cJSON_CreateObject();

    cJSON *username_item = cJSON_CreateString(auth_info->username);
    cJSON_AddItemToObject(json_object, USERNAME, username_item);

    cJSON *password_item = cJSON_CreateString(auth_info->password);
    cJSON_AddItemToObject(json_object, PASSWORD, password_item);

    return json_object;

}

void delete_auth_info(auth_info_t *auth_info) {

    if (auth_info == NULL) {
        return;
    }

    FREE(auth_info->username);
    FREE(auth_info->password);

    FREE(auth_info);

}





