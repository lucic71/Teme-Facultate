#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <stddef.h>
#include <authentication.h>

#include <cJSON.h>

typedef struct {
    char *name;
    char *value;
} rigid_json_t;

cJSON *array_to_cJSON_Object(rigid_json_t *array, size_t array_sz);
cJSON *convert_auth_info_to_cJSON(auth_info_t *auth_info, 
        char *username, char *password);

#endif

