#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "tcp_handler.hpp"
#include "helpers.h"
#include "protocol.hpp"

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

using namespace std;

int establish_tcp_connection(int port) {
    /*
     * Create listener socket.
     *
     */
    int listener_socket = socket(AF_INET, SOCK_STREAM, 0);
    DIE(listener_socket < 0, "server: tcp: socket");

    /*
     * Define the attributes of the socket.
     *
     */
    struct sockaddr_in server_addr_tcp;
    server_addr_tcp.sin_family      = AF_INET;
    server_addr_tcp.sin_addr.s_addr = INADDR_ANY;
    server_addr_tcp.sin_port        = htons(port);

    /*
     * Disable Nagle buffering using setsockopt with TCP_NODELAY option.
     * SO_REUSEADDR is used because we want to reuse a port shortly after
     * it was forecefully closed.
     *
     */
    int optname = 1;

    int setsockopt_ret;
    setsockopt_ret = setsockopt(listener_socket, IPPROTO_TCP,
            TCP_NODELAY | SO_REUSEADDR , &optname, sizeof(int));
    DIE(setsockopt_ret < 0, "server: tcp: setsockopt");

    /*
     * Bind the socket.
     *
     */
    int bind_ret_tcp;
    bind_ret_tcp = bind(listener_socket, (struct sockaddr *) &server_addr_tcp,
        sizeof(struct sockaddr_in));
    DIE(bind_ret_tcp < 0, "server: tcp: bind");

    /*
     * Listen for incoming connections.
     *
     */
    int listen_ret;
    listen_ret = listen(listener_socket, DEFAULT_BACKLOG);
    DIE(listen_ret < 0, "server: tcp: listen");

    return listener_socket;
}

int connect_to_server(char *server_ip, char *server_port) {
    int sockfd;
    struct sockaddr_in server_addr;

    /*
     * Convert the port.
     *
     */
    int port = atoi(server_port);
    DIE(port == 0, "client: atoi: invalid port");

    /*
     * Create the socket.
     *
     */
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    DIE(sockfd < 0, "client: socket");

    /*
     * Fill the info about the server the client want to connect to.
     *
     */
    server_addr.sin_family = PF_INET;
    server_addr.sin_port   = htons(port);

    int inet_aton_ret = inet_aton(server_ip, &server_addr.sin_addr);
    DIE(inet_aton_ret == 0, "client: inet_aton ");

    /*
     * Connect to server.
     *
     */
    int connect_ret = connect(sockfd, (struct sockaddr *) &server_addr,
        sizeof(struct sockaddr));
    DIE(connect_ret < 0, "client: connect");

    return sockfd;
}

int accept_connection(int listener, list<ClientIO>& clients) {

    struct sockaddr_in incoming_client;
    socklen_t incoming_client_len = sizeof(struct sockaddr_in);

    /*
     * Accept the connection using accept.
     *
     */
    int new_socket = accept(listener, (struct sockaddr *) &incoming_client,
        &incoming_client_len);
    ACCEPT_CONNECTION_ERROR(new_socket < 0, "server: accept");

    /*
     * Disable Nagle buffering using setsockopt with TCP_NODELAY option.
     *
     */
    int optname = 1;

    int setsockopt_ret;
    setsockopt_ret = setsockopt(new_socket, IPPROTO_TCP,
            TCP_NODELAY , &optname, sizeof(int));
    DIE(setsockopt_ret < 0, "server: tcp: setsockopt");

    /*
     * Request the name of the client. If the #request_client_id returns
     * an empty string then the connection closed and the connection cannot
     * be accepted
     *
     */
    string client_id = request_client_id(new_socket);
    if (client_id.empty() == true) {
        cout << "Connection closed while trying to request the id of the client"
            << endl;

        return ACCEPT_FAILED;
    }

    /*
     * If the id already exists then reject the connection and send
     * a reply about the rejection, else add the new id in the container.
     *
     */
    auto existent_client = find_if(clients.begin(), clients.end(),
        [client_id] (ClientIO& cli) {return cli.clientID.compare(client_id) == 0;});

    if (existent_client != clients.end()) {
        cout << "Client already connected, refusing connection." << endl;

        reply_connection_refused(new_socket);
        return ACCEPT_FAILED;
    }

    clients.push_back({client_id, new_socket});

    /*
     * Print info about the newly connected client.
     *
     */
    cout << "New client " << client_id << " connected from " << ntohs(incoming_client.sin_port)
        << ":" << inet_ntoa(incoming_client.sin_addr) << endl;

    return ACCEPT_SUCCESSFUL;
}
