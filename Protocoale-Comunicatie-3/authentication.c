#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#include <authentication.h>
#include <error.h>
#include <memory.h>
#include <prompt.h>

auth_info_t *get_auth_info() {
    char *username = read_item(USERNAME, MAX_USER_SZ, ECHO_ON);
    char *password = read_item(PASSWORD, MAX_PASS_SZ, ECHO_OFF);

    NONVOID_ERROR_HANDLER(username == NULL, "[ERROR] Could not read username", NULL);
    NONVOID_ERROR_HANDLER(password == NULL, "[ERROR] Could not read password", NULL);

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

void delete_auth_info(auth_info_t *auth_info) {

    FREE(auth_info->username);
    FREE(auth_info->password);

    FREE(auth_info);

}





