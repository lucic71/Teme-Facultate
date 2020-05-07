#include <iostream>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "tcp_handler.hpp"
#include "subscriber_utils.hpp"
#include "helpers.h"
#include "protocol.hpp"

using namespace std;

int main(int argc, char **argv) {

    if (argc < 4) {
        usage(argv[0]);
        exit(EXIT_SUCCESS);
    }

    char *client_id = argv[1];

    /*
     * Check the length of the id.
     *
     */
    if (strlen(client_id) > CLIENTID_MAX_LEN) {
        cout << "Client ID too long, aborting.." << endl;
        exit(EXIT_SUCCESS);
    }

    /*
     * Global buffer to save the data received via the connection established
     * by @socket_fd defined below.
     *
     */
    char buffer[BUFLEN];

    /*
     * Connect to server.
     *
     */
    int socket_fd = connect_to_server(argv[2], argv[3]);

    /*
     * Create fd set for I/O multiplexing.
     *
     */
    fd_set read_fds;
    int fd_max;

    while (true) {

        /*
         * Initialize the file descriptors set and call select for I/O multiplexing.
         * The subscriber only needs to know about standard input and socket.
         *
         */
        FD_ZERO(&read_fds);

        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(socket_fd,    &read_fds);

        fd_max = max(STDIN_FILENO, socket_fd);

        /*
         * Call select for I/O multiplexing.
         *
         */
        int read_action = select(fd_max + 1, &read_fds, NULL, NULL, NULL);
        DIE(read_action < 0, "client: select:");

        if (FD_ISSET(socket_fd, &read_fds)) {
            memset(buffer, 0x00, BUFLEN);

            uint8_t announce_byte;
            unsigned read_bytes = recv(socket_fd, &announce_byte, sizeof(announce_byte), 0);
            DIE(read_bytes < 0, "subscriber: recv: ");

            /*
             * Server shut down so clients will also shut down.
             *
             */
            if (read_bytes == 0) {
                cout << "Disconnecting.." << endl;
                break;
            }

            /*
             * Server received an id_request so it will respond with
             * its id.
             *
             */
            if (announce_byte == IDREQUEST_ANNOUNCEMENT_BYTE) {
                /*
                 * If the subscriber could not send its client id on the socket
                 * it means that the server closed unexpectedly and the subscriber
                 * must end the process.
                 *
                 */
                int rci_ret = reply_client_id(socket_fd, client_id);

                if (rci_ret == REPLY_FAILED) {
                    cout << "Client could not send its client id. Disconnecting.."
                        << endl;
                    break;
                }
            }

            /*
             * Server received the topic it is subscribed to.
             *
             */
            else if (announce_byte == TOPIC_ANNOUNCEMENT_BYTE) {
                /*
                 * Read all fields from topic, excluding value field.
                 *
                 */
                read_bytes = recv(socket_fd, buffer, sizeof(Topic) - VALUE_MAX_LEN, 0); 
                DIE(read_bytes < 0, "subscriber: recv: ");

                /*
                 * Create a topic object to store the received bytes.
                 *
                 */
                Topic topic;

                memset((void *) &topic, 0x00, sizeof(Topic));
                memcpy((void *) &topic, buffer, sizeof(Topic));

                /*
                 * If last recv(2) did not receive sizeof(Topic) - VALUE_MAX_LEN bytes
                 * then the subscriber must still receive bytes.
                 *
                 */
                if (read_bytes < sizeof(Topic) - VALUE_MAX_LEN) {
                    unsigned remaining = sizeof(Topic) - VALUE_MAX_LEN - read_bytes;

                    /*
                     * Start receiving remaining bytes from buffer + read_bytes.
                     *
                     */
                    read_bytes = recv(socket_fd, buffer + read_bytes, remaining, 0);
                    DIE(read_bytes < 0, "subscriber: recv: ");

                    memcpy((void *) &topic, buffer, sizeof(Topic));

                }

                size_t value_sz = topic.value_sz;

                /*
                 * Read the variable-sized field, value.
                 *
                 */

                read_bytes = recv(socket_fd, topic.value, value_sz, 0);
                DIE(read_bytes < 0, "subscriber: recv: ");

                /*
                 * Same story as above. If value_sz bytes were not received then receive
                 * the remaining bytes.
                 *
                 */
                if (read_bytes != value_sz) {
                    unsigned remaining = value_sz - read_bytes;

                    read_bytes = recv(socket_fd, topic.value + read_bytes, remaining, 0);
                    DIE(read_bytes < 0, "subscriber: recv: ");

                    if (read_bytes != remaining) {
                        cout << "Afara 2" << endl;
                        break;
                    }

                }

                /*
                 * Parse the topic and print its content.
                 *
                 */
                print_topic(&topic);

            } else if (announce_byte == CONNECTION_REFUSED_BYTE) {
                /*
                 * Notify the client that the clientID already exits.
                 *
                 */
                puts("ClientID already existent, retry with another clientID.");
                break;

            } else if (announce_byte == INVALID_REQUEST_ANNOUNCEMENT_BYTE) {
                /*
                 * Notift the client that the sent request was not a valid one.
                 *
                 */
                puts("Invalid request");
                continue;
            }

            

        } else if (FD_ISSET(STDIN_FILENO, &read_fds)) {
           memset(buffer, 0x00, BUFLEN);
           fgets(buffer, REQUEST_MAX_LEN, stdin);

           /*
            * Client wants to end the current session.
            *
            */
           if (strncmp(buffer, EXIT, strlen(EXIT)) == 0) {
               cout << "Disconnecting.." << endl;
               break;
           }

           /*
            * Send message to server.
            *
            */
           int write_bytes = send(socket_fd, buffer, REQUEST_MAX_LEN, 0);
           DIE(write_bytes < 0, "client: send");
        }

    }
        
    /*
     * Close the socket and exit.
     *
     */
    close(socket_fd);
    return EXIT_SUCCESS;

}
