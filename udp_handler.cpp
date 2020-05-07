#include <iostream>

#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

#include "udp_handler.hpp"
#include "helpers.h"


int establish_udp_connection(int port) {

    /*
     * Create UDP socket.
     *
     */
    int listener_socket = socket(AF_INET, SOCK_DGRAM, 0);
    DIE(listener_socket < 0, "server: udp: socket");

    /*
     * Define the attributes of the socket.
     *
     */
    struct sockaddr_in server_addr_udp;
    server_addr_udp.sin_family = AF_INET;
    server_addr_udp.sin_addr.s_addr = INADDR_ANY;
    server_addr_udp.sin_port = htons(port);

    /*
     * Bind the socket.
     *
     */
    int bind_ret_udp;
    bind_ret_udp = bind(listener_socket, (const struct sockaddr *) &server_addr_udp,
        sizeof(struct sockaddr_in));
    DIE(bind_ret_udp < 0, "server: udp: bind");

    return listener_socket;

}

Topic parse_datagram(int socket) {
    /*
     * Read the datagram coming from @socket in @buffer.
     *
     */
    char buffer[UDP_DATAGRAM_LEN];
    memset(buffer, 0x00, UDP_DATAGRAM_LEN);

    /*
     * Declare a sockaddr_in buffer for receiving information
     * about the upd_client such as port and ip.
     *
     */
    struct sockaddr_in incoming_client;
    socklen_t incoming_client_len = sizeof(struct sockaddr_in);

    int read_bytes = recvfrom(socket, buffer, UDP_DATAGRAM_LEN, 0,
            (struct sockaddr *) &incoming_client, &incoming_client_len);
    PARSE_DATAGRAM_ERROR(read_bytes < 0, "server: add_new_topic: read");

    /*
     * Create a new topic and fill its fields.
     *
     */
    Topic topic;
    memset(&topic, 0x00, sizeof(Topic));

    /*
     * IP
     *
     */
    topic.ip = incoming_client.sin_addr.s_addr;

    /*
     * PORT
     *
     */
    topic.port = ntohs(incoming_client.sin_port);

    /*
     * TOPIC
     *
     */
    strncpy(topic.topic, buffer, TOPIC);

    /*
     * DATA_TYPE
     *
     */
    topic.data_type = buffer[TOPIC];

    /*
     * VALUE
     *
     */
    if (topic.data_type == INT_BYTE) {

        topic.value_sz = INT_BYTE_VALUE_SIZE;
        memcpy(topic.value, buffer + TOPIC + 1, INT_BYTE_VALUE_SIZE);

    } else if (topic.data_type == SHORT_REAL_BYTE) {

        topic.value_sz = SHORT_REAL_BYTE_VALUE_SIZE;
        memcpy(topic.value, buffer + TOPIC + 1, topic.value_sz);

    } else if (topic.data_type == FLOAT_BYTE) {

        topic.value_sz = FLOAT_BYTE_VALUE_SIZE;
        memcpy(topic.value, buffer + TOPIC + 1, topic.value_sz);

    } else if (topic.data_type == STRING_BYTE) {

        topic.value_sz = strlen(buffer + TOPIC + 1);
        memcpy(topic.value, buffer + TOPIC + 1, topic.value_sz);

    } 

    /*
     * If data type is not a value that was defined in topic.hpp it means
     * that an overflow in topic.topic field occured and the received topic
     * is not a valid one. Return emtpy topic in this case.
     *
     */
    else {
        PARSE_DATAGRAM_ERROR(true, "overflow occured when receiving topic name");
    }

    return topic;
}




