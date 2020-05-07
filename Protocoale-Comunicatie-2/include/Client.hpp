#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <string>

/*
 * Definition of a client struct containg info about the
 * ID of the client, the file descriptor associated with this client
 * and the store&forward option. It is used for forwarding 
 *
 */
struct Client {
    std::string clientID;
    int fd;
    bool sf;
};

/*
 * Definition of a client struct containing info about the ID of the
 * client and the file descriptor assoiated with this client. It is
 * used for client-server communication.
 *
 */
struct ClientIO {
    std::string clientID;
    int fd;
};

#endif
