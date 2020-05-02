#ifndef AUTHENTICATION_H_
#define AUTHENTICATION_H_

#define MAX_USER_SZ    100
#define MAX_PASS_SZ    100

/*
 * Number of components used in a register/login operation:
 *  username
 *  psasword
 *
 */
#define AUTH_INFO_ELEMS 2

/*
 * Names of the fields in auth_info_t.
 *
 */
#define USERNAME "username"
#define PASSWORD "password"

typedef struct auth_info {
    char *username;
    char *password;
} auth_info_t;

/*
 * Display a prompt with username and password, let the user enter the
 * info and return it in a auth_info_t data type.
 *
 */
auth_info_t *get_auth_info();

/*
 * Frees the memory for a auth_info_t object.
 *
 */
void delete_auth_info(auth_info_t *auth_info);

#endif
