#include <iostream>
#include <algorithm>

#include <string.h>
#include <unistd.h>

#include "server_utils.hpp"
#include "protocol.hpp"

/*
 * Prototypes of local scope functions.
 *
 */

/*
 * Adds @topic in @pending_table in the entry associated with @client_id.
 * It is used in #forward_topic.
 *
 */
void pending_table_add(string client_id, Topic topic, 
        unordered_map<string, list<Topic>>& pending_table);

/*
 * Helper function that send a @topic on a socket associated with @fd.
 * It is configured to send a packet using the topic transmission
 * protocol.
 *
 * @return: SEND_SUCCESSFUL - the topic was successfully transmitted
 *          SEND_FAILED     - the connection was closed unexpectedly
 *
 */

#define SEND_SUCCESSFUL 0
#define SEND_FAILED     1 

int send_packet(int fd, Topic topic);

#define SEND_PACKET_ERROR(assertion, call_description)    \
    do {    \
        if (assertion) {    \
			fprintf(stderr, "(%s, %d): ",   \
					__FILE__, __LINE__);    \
			perror(call_description);   \
            return SEND_FAILED;     \
        }   \
    } while (0)


/*
 * Implementation of functions defined in protocol.hpp.
 *
 */

void usage(char *filename) {
    std::cout << "Usage: " << filename << " PORT" << std::endl;
}

void fd_set_add_clients(fd_set& set, int& max_fd, list<ClientIO> clients) {

    for (auto client = clients.begin(); client != clients.end(); client++) {
        int fd = (*client).fd;

        FD_SET(fd, &set);
        max_fd = max(max_fd, fd);
    }

}

void clients_close_connection(list<ClientIO> clients) {

    for (auto client : clients) {
        close(client.fd);
    }

}

void forward_topic(Topic topic, unordered_map<string, list<Client>>& topics_table,
        unordered_map<string, list<Topic>>& pending_table) {
    /*
     * Search for the topic in topics_table.
     */
    if (topics_table.find(topic.topic) == topics_table.end()) {
        return;
    }

    /*
     * Send the topic to all the subscribed clients.
     *
     */
    auto& clients = topics_table.at(topic.topic);
    for (auto& client : clients) {
        int fd = client.fd;

        /*
         * If the client is unavailable and has SF option on the add the packet
         * in the pending_table. Else if the SF option is not set, then skip.
         *
         */
        if (fd == UNAVAILABLE_CLIENT && client.sf == true) {

            pending_table_add(client.clientID, topic, pending_table);
            continue;

        } else if (fd == UNAVAILABLE_CLIENT && client.sf == false) {
            continue;
        }

        /*
         * If #send_packet failed then unvalidate the client.
         *
         */
        int send_packet_ret = send_packet(fd, topic);
        if (send_packet_ret == SEND_FAILED) {
            client.fd = UNAVAILABLE_CLIENT;
        }

    }

}

int forward_pending_topics(int fd, string client_id,
        unordered_map<string, list<Topic>>& pending_table) {

    int return_code = FORWARD_SUCCESSFUL;

    if (pending_table.find(client_id) == pending_table.end()) {
        cout << "Nothing to forward from pending table at the moment" << endl;
        return return_code;
    }

    auto& topics = pending_table.at(client_id);
    for (auto& topic : topics) {

        /*
         * If the topics cannot be sent then an error persists on the
         * network and all topics will be dropped.
         *
         */
        int send_packet_ret = send_packet(fd, topic);
        if (send_packet_ret == SEND_FAILED) {
            cout << "Error while trying to send pending packets for client "
                << client_id << ". Aborting this operation.." << endl;

            return_code = FORWARD_FAILED;
            break;
        }

    }

    pending_table.erase(client_id);
    return return_code;
}

void unvalidate_client(int client_fd, 
        unordered_map<string, list<Client>>& topics_table) {

    for (auto& t_clients : topics_table) {
        for (auto& client : t_clients.second) {

            if (client.fd == client_fd) {
                client.fd = UNAVAILABLE_CLIENT;
            }

        }
    }

}

bool validate_client(string client_id, int new_fd,
        unordered_map<string, list<Client>>& topics_table) {

    /*
     * Keep a counter that counts how many times a client was validated.
     * If the counter not equal to 0 it means that the client reconnected
     * and was validated.
     *
     */
    int count(0);

    bool sf(false);

    for (auto& t_clients : topics_table) {
        for (auto& client : t_clients.second) {

            if (client.clientID.compare(client_id) == 0) {
                client.fd = new_fd;

                count++;
                sf = client.sf;
            }

        }
    }

    if (count != 0) {
        cout << "File descriptor for client " << client_id <<
            " was restored." << endl;
    }

    return sf;
}

void delete_invalid_clients(list<Client> subscribers, list<ClientIO>& clientsIO) {
    /*
     * Iterate through the subscribers and check if fd field is UNAVAILABLE_CLIENT.
     * Find the clientID in clientsIO and delete that entry.
     *
     */
    for (auto subscriber : subscribers) {

        if (subscriber.fd == UNAVAILABLE_CLIENT) {

            auto client = find_if(clientsIO.begin(), clientsIO.end(),
                [subscriber] (ClientIO& c) 
                    {return c.clientID.compare(subscriber.clientID) == 0; });

            if (client != clientsIO.end()) {
                clientsIO.erase(client);
            }
        }

    }

}


void pending_table_add(string client_id, Topic topic, 
        unordered_map<string, list<Topic>>& pending_table) {

    /*
     * If the @client_id is not present in @pending_table then add the client.
     *
     */
    if (pending_table.find(client_id) == pending_table.end()) {
        pending_table.insert({client_id, {}});
    }

    /*
     * Add @topic to the pending list of @client_id.
     *
     */
    auto& pending_table_entry = pending_table.at(client_id);
    pending_table_entry.push_back(topic);

}


int send_packet(int fd, Topic topic) {

    /*
     * Announce the subscriber that a topic is coming.
     *
     */
    uint8_t announce_byte = TOPIC_ANNOUNCEMENT_BYTE;

    int write_bytes = send(fd, &announce_byte, sizeof(announce_byte), 0);
    SEND_PACKET_ERROR(write_bytes < 0, "server_utils: send_packet: send: ");

    /*
     * Send all fields from Topic, excluding value because it has variable size and
     * must send topic.value_sz bytes.
     *
     */
    char buffer[sizeof(Topic) - VALUE_MAX_LEN];
    memcpy(buffer, (const void *) &topic, sizeof(Topic) - VALUE_MAX_LEN);

    write_bytes = send(fd, buffer, sizeof(Topic) - VALUE_MAX_LEN, 0);
    SEND_PACKET_ERROR(write_bytes < 0, "server_utils: send_packet: send: ");

    /*
     * Send the value field.
     *
     */
    char buffer_value[VALUE_MAX_LEN];

    size_t value_sz = topic.value_sz;
    memcpy(buffer_value, topic.value, value_sz);

    write_bytes = send(fd, buffer_value, value_sz, 0);
    SEND_PACKET_ERROR(write_bytes < 0, "server_utils: send_packet: send: ");

    return SEND_SUCCESSFUL;
}
