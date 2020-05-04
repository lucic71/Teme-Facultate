#ifndef SERVER_UTILS_HPP_
#define SERVER_UTILS_HPP_

#include <unordered_map>
#include <list>
#include <string>

#include "topic.hpp"
#include "Client.hpp"
#include "helpers.h"

#include "sys/socket.h"

using namespace std;

#define EXIT_COMMAND "exit"

#define UNAVAILABLE_CLIENT -1

/*
 * Prints the usage if the number of arguments is not correct.
 *
 */
void usage(char *filename);

/*
 * Forwards the @topic to all the clients subscribed to it. Also updates
 * the @pending_table if there are unavailable clients with the SF option on.
 *
 * If a client from @topics_table becomes unavailable while the topic is
 * transmitted then the entry will be invalidated using UNAVAILABLE_CLIENT.
 *
 */
void forward_topic(Topic topic, unordered_map<string, list<Client>>& topics_table,
        unordered_map<string, list<Topic>>& pending_table);

/*
 * After a client reconnects it must receive the pending packets associated
 * with its @client_id. If the client disconnects during this operation, then
 * the function will return an error value. Furthermore if the client
 * disconnects during the forwarding then all pending packets will be
 * dropped.
 *
 * @return: FORWARD_SUCCESSFUL when the operation finishes successfully
 *          FORWARD_FAILED     the client disconnected during this operation
 *
 */
#define FORWARD_SUCCESSFUL 0
#define FORWARD_FAILED     1

int forward_pending_topics(int fd, string client_id,
        unordered_map<string, list<Topic>>& pending_table);

/*
 * Adds a all @clients to @set for further I/O multiplexing.
 * Also update max_fd.
 *
 */
void fd_set_add_clients(fd_set& set, int& max_fd, list<ClientIO> clients);

/*
 * Close the connection with each client from @clients.
 *
 */
void clients_close_connection(list<ClientIO> clients);

/*
 * Traverse @topics_table and make the client unavailable by setting its file descriptor
 * field to UNAVAILABLE_CLIENT.
 *
 */
void unvalidate_client(int client_fd, 
        unordered_map<string, list<Client>>& topics_table);

/*
 * Traverse @topics_table and make the client available by setting its flie descriptor
 * field to @new_fd.
 *
 * @return: the sf option of the validated client. It returns the sf option because there
 * is no other palace where this option is stored and the server main function needs
 * to know if it has to flush @pending_topics for @client_id.
 *
 */
bool validate_client(string client_id, int new_fd,
        unordered_map<string, list<Client>>& topics_table);

/*
 * After #forward_topic function is called, some clients may be invalidated, so
 * their entry must be deleted from @clientsIO.
 *
 */
void delete_invalid_clients(list<Client> subscribers, list<ClientIO>& clientsIO);

#endif
