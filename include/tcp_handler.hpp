#ifndef HANDLER_TCP_HPP_
#define HANDLER_TCP_HPP_

#include <list>
#include <vector>

#include "Client.hpp"

#define DEFAULT_BACKLOG 32

using namespace std;

/*
 * Set up a TCP connection by returing a file descriptor
 * that points to a socket that listens for new TCP connections.
 *
 */
int establish_tcp_connection(int port);

/*
 * Helps a client to connect to the server specified by @server_ip
 * and @server_port.
 *
 */
int connect_to_server(char *server_ip, char *server_port);

/*
 * Accept an incomming connection from @listener and insert its file descriptor
 * in @clients_fd.
 *
 * @return; ACCEPT_SUCCESSFUL
 *          ACCEPT_FAILED
 *
 */

#define ACCEPT_SUCCESSFUL 0
#define ACCEPT_FAILED     1

#define ACCEPT_CONNECTION_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
            return ACCEPT_FAILED;    \
        }   \
    } while (0)

int accept_connection(int listener, list<ClientIO>& client);

#endif
