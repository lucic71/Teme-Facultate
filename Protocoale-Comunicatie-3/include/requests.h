#ifndef REQUESTS_H_
#define REQUESTS_H_

/*
 * Computes a GET request addressed to the @url hosted by @host.
 *
 * @query_params and @cookies can be set to NULL if not needed.
 *
 * @return: a char array containing the GET request.
 *          NULL if the operation faield (input was invalid)
 *
 */
char *compute_get_request(char *host, char *url, char *query_params,
							char **cookies, int cookies_count);

/*
 * Computes a POST request addressed to the @url hosted by @host.
 *
 * @cookies can be set to NULL if not needed.
 * @body_data is the data contained in the POST request.
 *
 * @return: a char array containing the POST request.
 *
 */
char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
							int body_data_fields_count, char** cookies, int cookies_count);

/*
 * Adds a x-access-token field in the header of the GET request.
 *
 */
char *add_access_token(char *request, char *jwt_token);

#endif
