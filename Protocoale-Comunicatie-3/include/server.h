#ifndef SERVER_H_
#define SERVER_H_

/*
 * Info about the server.
 *
 */
#define SERVER "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define PORT   8080

/*
 * Content types.
 *
 */
#define JSON_CONTENT_TYPE "application/json"

/*
 * URLs.
 *
 */
#define REGISTER_URL      "/api/v1/tema/auth/register"
#define LOGIN_URL         "/api/v1/tema/auth/login"
#define ENTER_LIBRARY_URL "/api/v1/tema/library/access"
#define GET_BOOKS_URL     "/api/v1/tema/library/books"

#endif
