#ifndef DNS_H_
#define DNS_H_

#include <stdint.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

/*
 * Get the IPv4 address in integer format.
 * Returns NULL on error.
 *
 */

char *get_ip(char *name);

#endif
