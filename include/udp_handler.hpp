#ifndef HANDLER_UDP_HPP_
#define HANDLER_UDP_HPP_

#include <string>
#include "topic.hpp"

using namespace std;

/*
 * Used for receiving a datagram in #parse_datagram.
 *
 */
#define TOPIC 50
#define DATA_TYPE 1
#define CONTENT   1500

#define UDP_DATAGRAM_LEN (TOPIC + DATA_TYPE + CONTENT)

/*
 * Set up a UDP connections by returning a file descriptor that points
 * to a socket that listens for UDP datagrams.
 *
 */
int establish_udp_connection(int port);

/*
 * Receive a datagram from @socket and parse to be compatible with the
 * Topic format (see include/topic.hpp).
 *
 */

#define PARSE_DATAGRAM_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
            return {};  \
        }   \
    } while (0)

Topic parse_datagram(int socket);

#endif
