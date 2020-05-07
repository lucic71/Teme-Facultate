#include <iostream>

#include <string.h>
#include <unistd.h>

#include "server_utils.hpp"
#include "tcp_handler.hpp"
#include "udp_handler.hpp"
#include "helpers.h"
#include "protocol.hpp"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    /*
     * Initialize the sockets used for TCP and UDP communication.
     *
     */
    int port = atoi(argv[1]);
    DIE(port == 0, "server: atoi");

    int tcp_listener = establish_tcp_connection(port);
    int udp_listener = establish_udp_connection(port);

    /*
     * Keep a list of file descriptors associated with each TCP client
     * that is connected to server.
     *
     */
    list<ClientIO> clients; 

    /*
     * Keep a table that will save the clients subscribed to different
     * topics.
     *
     */
    unordered_map<string, list<Client>> topics_table;

    /*
     * Keep a table that will save for each user the packets to be
     * transmitted after a client reconnects and has SF option on.
     *
     */
    unordered_map<string, list<Topic>> pending_table;

    /*
     * File descriptors used for I/O multiplexing.
     *
     */
    fd_set read_fds;
    int max_fd;

    while (true) {
        /*
         * Initialize the file descriptors set and call select for I/O multiplexing.
         * Listen on udp, tcp accept and stdin. Also listen on the connections 
         * established with the clients.
         *
         */
        FD_ZERO(&read_fds);

        FD_SET(tcp_listener, &read_fds);
        FD_SET(udp_listener, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        max_fd = max(tcp_listener, udp_listener);
        max_fd = max(STDIN_FILENO, max_fd);

        fd_set_add_clients(read_fds, max_fd, clients);

        /*
         * Call select for I/O multiplexing.
         *
         */
        int read_activity;
        read_activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        DIE(read_activity < 0, "server: select");

        /*
         * If something happened on tcp_listener_socket it means that a new connection
         * waits to be accepted.
         *
         */
        if (FD_ISSET(tcp_listener, &read_fds)) {
            /*
             * If accept_connection was not successful, then skip
             * the operations below.
             *
             */
            int ac_ret = accept_connection(tcp_listener, clients);

            if (ac_ret == ACCEPT_SUCCESSFUL) {
                /*
                 * Maybe the client reconnected, so the server must validate its
                 * entry in topics table. The entry was unvalidated by #unvalidate_client.
                 * It(unvalidate_client) is called when resolve_subscribe_request 
                 * returns a CONNECTION_CLOSED_ERROR value.
                 *
                 */
                string last_connected_id = clients.back().clientID;
                int    last_connected_fd = clients.back().fd;
                bool sf = validate_client(last_connected_id, last_connected_fd, topics_table);

                /*
                 * If the client has the SF option on then send the topics in 
                 * @pending_table assigned to it.
                 *
                 */
                if (sf == true) {
                    int fpt_ret = forward_pending_topics(last_connected_fd, 
                            last_connected_id, pending_table);

                    /*
                     * If forwarding failed the remove the client.
                     *
                     */
                    if (fpt_ret == FORWARD_FAILED) {

                        cout << "Client removed after pending topics forwarding was not"
                            << " successful." << endl;

                        auto last_connected = --clients.end();
                        clients.erase(last_connected);

                        unvalidate_client(last_connected_fd, topics_table);

                    }
                }
            }

        }

        /*
         * Check the rest of the file descriptors in read_fds. Here the server
         * resolves the subscribe and unsubscribe requests.
         *
         */
        auto client = clients.begin();
        while (client != clients.end()) {

            int fd = (*client).fd;
            if (FD_ISSET(fd, &read_fds)) {
                int rsr_ret = resolve_subscribe_request(fd, topics_table);

                /*
                 * If the client has disconnected then delete its entry
                 * from @clients. Also make its entry from @topics_table
                 * unavailable to let the #forward_topic function know that
                 * it should not send a topic on the file descriptor associated
                 * with this client.
                 *
                 */
                if (rsr_ret == CONNECTION_CLOSED_ERROR) {
                    cout << "Client " << (*client).clientID << 
                        " made unavailable until reconnection" << endl;

                    unvalidate_client((*client).fd, topics_table);

                    close(fd);

                    client = clients.erase(client);
                    continue;
                }
            }

            client++;

        }

        /*
         * If something happened on udp_listener_socket it means that a new topic arrived
         * and waits to be forwarded to its subscribers.
         *
         */
        if (FD_ISSET(udp_listener, &read_fds)) {
            Topic topic = parse_datagram(udp_listener);
            /*
             * If #parse_datagram returns an empty topic it means an error occured and the
             * operation will be skipped.
             *
             */
            Topic empty_topic{};
            if (topic == empty_topic) {
                continue;
            }

            forward_topic(topic, topics_table, pending_table);

            /*
             * Delete invalid clients. These are the clients that disconnected
             * during #forward_topic. The condition in if assures that the topic
             * for which we delete the invalid clients appears in @topics_table.
             *
             */
            if (topics_table.find(topic.topic) != topics_table.end()) {
                delete_invalid_clients(topics_table.at(topic.topic), clients);
            }
        }

        /*
         * If something happened on stdin then the server must be closed. 
         *
         */
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char buffer[sizeof(EXIT_COMMAND) + 1];
            memset(buffer, 0x00, sizeof(EXIT_COMMAND) + 1);

            fgets(buffer, sizeof(EXIT_COMMAND), stdin);

            /*
             * End the session if the server receives EXIT_COMMAND.
             *
             */
            if (strncmp(buffer, EXIT_COMMAND, sizeof(EXIT_COMMAND)) == 0) {
                cout << "Disconnecting.." << endl;
                break;
            }
        }

    }
        
    /*
     * Close the connection with the tcp clients.
     *
     */
    clients_close_connection(clients);
    close(tcp_listener);
    close(udp_listener);

    return EXIT_SUCCESS;
}
