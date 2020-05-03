#include <stdio.h>

#include <dns.h>
#include <error.h>

char *get_ip(char *name) {

    /*
     * Set the hints for getaddrinfo(3).
     *
     */
    struct addrinfo hints = {
        .ai_flags     = 0,
        .ai_family    = AF_INET,
        .ai_socktype  = SOCK_DGRAM,
        .ai_protocol  = 0,

        .ai_addrlen   = 0,
        .ai_addr      = NULL,
        .ai_canonname = NULL,
        .ai_next      = NULL,
    };

    /*
     * Call getaddrinfo(3) and store the info in @result.
     *
     */
    struct addrinfo *result;
    int getaddrinfo_ret = getaddrinfo(name, NULL, &hints, &result);
    ERROR_HANDLER(getaddrinfo_ret < 0, NULL);

    /*
     * Get the IPv4 address from @result.
     *
     */
    struct sockaddr_in *ipv4_addr = (struct sockaddr_in *) result->ai_addr;
    uint32_t ip_addr = ipv4_addr->sin_addr.s_addr;

    freeaddrinfo(result);

    /*
     * Return the IPv4 address converted to char array.
     *
     */
    return inet_ntoa((struct in_addr) { .s_addr = ip_addr});
}
