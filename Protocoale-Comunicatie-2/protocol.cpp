#include <cstdlib>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iostream>

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "protocol.hpp"
#include "helpers.h"

string request_client_id(int client) {
    /*
     * Announce the client that it receives an id request.
     *
     */
    uint8_t announce_byte = IDREQUEST_ANNOUNCEMENT_BYTE;

    int write_bytes = send(client, &announce_byte, sizeof(announce_byte), 0);
    REQUEST_CLIENT_ID_ERROR(write_bytes < 0, "server: request_client_id: write");

    /*
     * Wait for the reply.
     *
     */
    char buffer[CLIENTID_MAX_LEN + 1];
    memset(buffer, 0x00, CLIENTID_MAX_LEN + 1);

    int read_bytes = recv(client, buffer, CLIENTID_MAX_LEN, 0);
    REQUEST_CLIENT_ID_ERROR(read_bytes < 0, "server: request_client_id: read");

    return {buffer};
}

int reply_client_id(int socket, char *client_id) {
    char buffer[CLIENTID_MAX_LEN + 1];

    memset(buffer, 0x00, CLIENTID_MAX_LEN + 1);
    strcpy(buffer, client_id);

    int write_bytes = send(socket, buffer, CLIENTID_MAX_LEN, 0);
    REPLY_CLIENT_ID_ERROR(write_bytes < 0, "client: reply_client_id: send:");

    return REPLY_SUCCESSFUL;
}

void reply_connection_refused(int socket) {

    uint8_t announce_byte = CONNECTION_REFUSED_BYTE;
    int write_bytes = send(socket, &announce_byte, sizeof(announce_byte), 0);
    REPLY_CONNECTION_REFUSED_ERROR(write_bytes < 0,
            "server: reply_connection_refused: send ");

}

void reply_invalid_request(int socket) {

    uint8_t announce_byte = INVALID_REQUEST_ANNOUNCEMENT_BYTE;
    int write_bytes = send(socket, &announce_byte, sizeof(announce_byte), 0);
    REPLY_INVALID_REQUEST_ERROR(write_bytes < 0,
            "server: reply_connection_refused: send ");

}

int resolve_subscribe_request(int client, 
        unordered_map<string, list<Client>>& topics_table) {

    char buffer[BUFLEN];
    memset(buffer, 0x00, BUFLEN);

    /*
     * Read the request from client.
     *
     */
    int read_bytes = recv(client, buffer, REQUEST_MAX_LEN, 0);
    RESOLVE_SUBSCRIBE_REQUEST_ERROR(read_bytes < 0, 
            "server: resolve_subscribe_request: read");

    /*
     * If 0 bytes were received it means that the client disconnected and
     * the server must close the file desciptor associated.
     *
     */
    if (read_bytes == 0) {
        return CONNECTION_CLOSED_ERROR;
    }

    /*
     * Request the id. If #request_client_id returns an empty string
     * then the connection closed and #resolve_subscribe_request will
     * return CONNECTION_CLOSED_ERROR.
     *
     */
    string client_id = request_client_id(client);
    if (client_id.empty() == true) {
        return CONNECTION_CLOSED_ERROR;
    }


    /*
     * Split the request in separate tokens.
     *
     */
    istringstream  request_stream(buffer);
    vector<string> request(istream_iterator<string>{request_stream},
            istream_iterator<string>{});

    /*
     * Process the SUBSCRIBE and UNSUBSCRIBE requests.
     *
     */
    if (request.size() == SUBSCRIBE_REQUEST_LENGTH) {
        /*
         * Parse the request.
         *
         */
        string request_type = request.at(0);
        string topic        = request.at(1);
        string sf           = request.at(2);

        /*
         * If topic length is bigger than TOPIC_MAX_LEN then it is an
         * invalid requet.
         *
         */
        if (topic.length() > TOPIC_MAX_LEN) {
            cout << "Topic size too big. Aborting operation.." << endl;

            reply_invalid_request(client);
            return INVALID_REQUEST_ERROR;
        }

        /*
         * Process a subscribe request.
         *
         */
        if (request_type.compare({SUBSCRIBE}) == 0) {

            /*
             * Check if the @topic can be found in @topics_table.
             * If not insert it, else use the existing one.
             *
             */
            if (topics_table.find(topic) == topics_table.end()) {
                topics_table.insert({topic, {}});
            }

            auto& topics_table_entry = topics_table.at(topic);

            /*
             * Check if the client alrady exists in @topics_table_entry.
             *
             * If it exists and has the same SF option then return with
             * REQUEST_SUCCESSFUL and do nothing, else if the SF option
             * is different then update the SF for this client at this
             * topic and return REQUEST_SUCCESSFUL.
             *
             */
            for (auto& topics_table_client : topics_table_entry) {

                if (topics_table_client.clientID.compare(client_id) == 0 
                        and stoi(sf) == topics_table_client.sf) {
                    
                    cout << "Client " << client_id << " already subscribed to "
                        << topic << ". Skipping this operation,," << endl;
                    return REQUEST_SUCCESSFUL;

                } else if (topics_table_client.clientID.compare(client_id) == 0 
                        and stoi(sf) != topics_table_client.sf) {

                    /*
                     * Check SF to be a valid option.
                     *
                     */
                    if (stoi(sf) != stoi(SF_ON) && stoi(sf) != stoi(SF_OFF)) {

                        reply_invalid_request(client);
                        cout << "Invalid subscribe request: SF" << endl;
                        return INVALID_REQUEST_ERROR;

                    }


                    topics_table_client.sf = stoi(sf);

                    cout << "SF option was updated for client " << client_id
                        << " who was subscribed to " << topic << endl;
                    return REQUEST_SUCCESSFUL;
                }

            }

            /*
             * Process store and forward option.
             *
             */
            if (sf.compare({SF_ON}) == 0) {

                topics_table_entry.push_back({client_id, client, 1});

                cout << "Client " << client_id << " subscribed successfully to topic "
                    << topic << " with SF option: 1" << endl;
                return REQUEST_SUCCESSFUL;

            } else if (sf.compare({SF_OFF}) == 0) {

                topics_table_entry.push_back({client_id, client, 0});

                cout << "Client " << client_id << " subscribed successfully to topic "
                    << topic << " with SF option: 0" << endl;
                return REQUEST_SUCCESSFUL;

            } else {

                reply_invalid_request(client);
                cout << "Invalid subscribe request: SF" << endl;
                return INVALID_REQUEST_ERROR;

            }


        } else {

            reply_invalid_request(client);
            cout << "Invalid request" << endl;
            return INVALID_REQUEST_ERROR;

        }


    } else if(request.size() == UNSUBSCRIBE_REQUEST_LENGTH) {
        /*
         * Parse the request.
         *
         */
        string request_type = request.at(0);
        string topic        = request.at(1);

        /*
         * Process an unsubscribe request.
         *
         */
        if (request_type.compare({UNSUBSCRIBE}) == 0) {

            /*
             * Check if the @topic can be found in @topics_table.
             *
             */
            if(topics_table.find(topic) == topics_table.end()) {
                cout << "Topic cannot be unsubscribed because it does not exists" << endl;

                reply_invalid_request(client);
                return INVALID_REQUEST_ERROR;
            }

            /*
             * Find the topic to which the client was subscribed.
             *
             */
            auto topics_table_entry = topics_table.at(topic);

            auto remove_iter = remove_if(topics_table_entry.begin(),
                   topics_table_entry.end(),
                   [client] (Client& lst_client) {return lst_client.fd == client;});

            /*
             * If the client was not subscribed to this topic then skip, else
             * delete its corresponding Client entry from the list of clients
             * subscribed to this topic.
             *
             */
            if (remove_iter == topics_table_entry.end()) {

                reply_invalid_request(client);
                cout << "Cannot remove client from this topic because he was not"
                    << " subscribed" << endl;
                return INVALID_REQUEST_ERROR;

            } else {

                topics_table_entry.erase(remove_iter);

                cout << "Client " << client_id << " unsubscribed successfully from topic "
                    << topic << endl;
                return REQUEST_SUCCESSFUL;

            }

        } else {

            reply_invalid_request(client);
            cout << "Invalid request" << endl;
            return INVALID_REQUEST_ERROR;

        }

    } else {

        reply_invalid_request(client);
        cout << "Invalid request" << endl;
        return INVALID_REQUEST_ERROR;

    }

}

