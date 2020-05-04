#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

#include <string>
#include <unordered_map>
#include <list>
#include <Client.hpp>

#include "topic.hpp"

using namespace std;


/*
 * Bytes that notify the subscriber what kind of message it
 * is about to receive.
 *
 */
#define IDREQUEST_ANNOUNCEMENT_BYTE       0xFF
#define TOPIC_ANNOUNCEMENT_BYTE           1
#define CONNECTION_REFUSED_BYTE           2
#define INVALID_REQUEST_ANNOUNCEMENT_BYTE 3

/*
 * Maximum size of data that the protocol can operate with.
 *
 */
#define BUFLEN (sizeof (Topic))

/*
 * Maximum length of a client id.
 *
 */
#define CLIENTID_MAX_LEN 10



/*
 * Format of subscribe and unsubscribe requests.
 *
 */
#define SUBSCRIBE   "subscribe"
#define UNSUBSCRIBE "unsubscribe"

/*
 * Format of store and forward options.
 *
 */
#define SF_ON  "1"
#define SF_OFF "0" 

/*
 * Maximul length (in bytes) of a un/subscribe request.
 * Length of UNSUBSCRIBE + length of SF option + maximum length of a topic
 * + 2 spaces.
 *
 */
#define REQUEST_MAX_LEN (sizeof(UNSUBSCRIBE) - 1 + \
        sizeof(SF_ON) - 1 + TOPIC_MAX_LEN + sizeof(uint8_t) * 2)

/*
 * Lengths (in words separated by space) of subscribe and unsubscribe requests.
 *
 */
#define SUBSCRIBE_REQUEST_LENGTH   3
#define UNSUBSCRIBE_REQUEST_LENGTH 2

/*
 * Makes a request for the client_id to a client described by a file 
 * descriptor @client. It has also a error handler macro associated.
 * If a send or recv call fails then the function will return an
 * empty string and a error mesage will be displayed.
 *
 */
string request_client_id(int client);

#define REQUEST_CLIENT_ID_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
            return {};  \
        }   \
    } while (0)

/*
 * Reply for the above mentioned request. The @client_id is transmited
 * via the connection established by @socket.
 *
 * @return: REPLY_SUCCESSFUL if the subscriber sent bytes on the socket
 *          REPLY_FAILED     if the subscriber failed to send bytes
 *
 */
#define REPLY_SUCCESSFUL 0
#define REPLY_FAILED     1

#define REPLY_CLIENT_ID_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
            return REPLY_FAILED;    \
        }   \
    } while (0)

int reply_client_id(int socket, char *client_id);

/*
 * Reply with a CONNECTION_REFUSED_BYTE from server to let the client
 * know that there is already a client with the same clientID.
 *
 */

#define REPLY_CONNECTION_REFUSED_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
        }   \
    } while (0)

void reply_connection_refused(int socket);

/*
 * Reply with a INVALID_REQUEST_ANNOUNCEMENT_BYTE from server to let the cient
 * know that the un/subscribe request is sent is invalid.
 *
 */
#define REPLY_INVALID_REQUEST_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
        }   \
    } while (0)

void reply_invalid_request(int socket);

/*
 * When a subscriber sends a subscribe request, this function will resolve
 * the request by adding the client in @topics_table.
 *
 * @return: CONNECTION_CLOSED_ERROR  - connection was closed and the request 
 *                                   could not be processed
 *          INVALID_REQUEST_ERROR    - the request does not have the falid format
 *          REQUEST_SUCCESSFUL       - the request was successfully 
 *                                   processed
 *
 * The error macro is used when the function cannot read data from the
 * socket.
 *
 */
#define REQUEST_SUCCESSFUL       1
#define CONNECTION_CLOSED_ERROR  0
#define INVALID_REQUEST_ERROR   -1

int resolve_subscribe_request(int client,
        unordered_map<string, list<Client>>& topics_table);

#define RESOLVE_SUBSCRIBE_REQUEST_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
            return CONNECTION_CLOSED_ERROR;    \
        }   \
    } while (0)

#endif
