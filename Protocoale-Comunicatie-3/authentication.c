#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#include <authentication.h>
#include <error.h>
#include <memory.h>

auth_info_t *get_auth_info() {
    /*
     * Read the username.
     *
     */
    printf("username: ");
    fflush(stdout);

    char username[MAX_USER_SZ] = {0};
    fgets(username, MAX_USER_SZ - 1, stdin);
    username[strlen(username) - 1] = 0x00;

    /*
     * Disable terminal echoing for reading password.
     *
     */
    struct termios current, modified;

    // get current attributes
    int tcgetattr_ret = tcgetattr(STDIN_FILENO, &current);
    NONVOID_ERROR_HANDLER(tcgetattr_ret == -1, "get_auth_info: tcgetattr", NULL);

    modified = current;
    modified.c_lflag &= ~ECHO;

    int tcsetattr_ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &modified);
    NONVOID_ERROR_HANDLER(tcsetattr_ret == -1, "get_auth_info: tcsetattr", NULL);

    /*
     * Read the password.
     *
     */
    printf("password: ");
    fflush(stdout);

    char password[MAX_USER_SZ] = {0};
    fgets(password, MAX_PASS_SZ - 1, stdin);
    password[strlen(password) - 1] = 0x00;

    puts("");

    /*
     * Restore original settings for echoing.
     *
     */
    tcsetattr_ret = tcsetattr(STDIN_FILENO, TCSANOW, &current);
    NONVOID_ERROR_HANDLER(tcsetattr_ret == -1, "get_auth_info: tcsetattr", NULL);

    /*
     * Declare the return variable.
     *
     */
    auth_info_t *auth_info;
    CALLOC(auth_info, sizeof(auth_info_t));

    CALLOC(auth_info->username, strlen(username) + 1);
    strcpy(auth_info->username, username);

    CALLOC(auth_info->password, strlen(password) + 1);
    strcpy(auth_info->password, password);

    return auth_info;

}

void delete_auth_info(auth_info_t *auth_info) {

    FREE(auth_info->username);
    FREE(auth_info->password);

    FREE(auth_info);

}





