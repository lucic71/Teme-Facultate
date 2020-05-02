#include <json_parser.h>

cJSON *array_to_cJSON_Object(rigid_json_t *array, size_t array_sz) {

    cJSON *json_object = cJSON_CreateObject();

    for (size_t i = 0; i < array_sz; i++) {
        cJSON *item = cJSON_CreateString(array[i].value);
        cJSON_AddItemToObject(json_object, array[i].name, item);
    }

    return json_object;
}

cJSON *convert_auth_info_to_cJSON(auth_info_t *auth_info, 
        char *username, char *password) {

    /*
     * Create an array of rigid_json_t's and convert them using
     * #array_to_cJSON_Object.
     *
     */
    rigid_json_t json_array_content[AUTH_INFO_ELEMS];

    json_array_content[0] = (rigid_json_t) 
        { .name = username, .value = auth_info->username };
    json_array_content[1] = (rigid_json_t) 
        { .name = password, .value = auth_info->password };

    return array_to_cJSON_Object(json_array_content, AUTH_INFO_ELEMS);

}
