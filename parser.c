#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "parser.h"

#define RTABLE_FILE "rtable.txt"
#define ADDR_MAX_LEN ((16) + (1))
#define BITS_MAX_LEN 33

void read_rtable(trie_node_t **rtable) {

    // guard
    if (rtable == NULL) {
        perror("read_rtable: rtable pointer is null");
        exit(EXIT_FAILURE);
    }

    *rtable = create_node();

    // open the file
    FILE *rtable_file = fopen(RTABLE_FILE, "r");
    if (rtable_file == NULL) {
        perror("count_lines_nr: could not open the file");
        exit(EXIT_FAILURE);
    }

    // variables used for getline
    char *file_line = NULL;
    size_t len = 0;
    int read;

    char *prefix_str   = calloc(1, ADDR_MAX_LEN);
    char *next_hop_str = calloc(1, ADDR_MAX_LEN);
    char *mask_str     = calloc(1, ADDR_MAX_LEN);
    int interface;

    // read the file line by line and insert the prefixes in a trie
    while ((read = getline(&file_line, &len, rtable_file)) != -1) {
        sscanf(file_line, "%s %s %s %d", prefix_str, next_hop_str, mask_str, &interface);

        uint32_t h_prefix   = inet_network(prefix_str);
        uint32_t h_mask     = inet_network(mask_str);

        uint32_t n_prefix   = inet_addr(prefix_str);
        uint32_t n_next_hop = inet_addr(next_hop_str);
        uint32_t n_mask     = inet_addr(mask_str);

        rtable_entry_t entry;
        entry.prefix = n_prefix;
        entry.next_hop = n_next_hop;
        entry.mask = n_mask;
        entry.interface = interface;

        insert_node(*rtable, h_prefix & h_mask, __builtin_popcount(h_mask), &entry);

        memset(prefix_str  , 0x00, ADDR_MAX_LEN);
        memset(next_hop_str, 0x00, ADDR_MAX_LEN);
        memset(mask_str    , 0x00, ADDR_MAX_LEN);
    }

    free(prefix_str);
    free(next_hop_str);
    free(mask_str);
    free(file_line);

    fclose(rtable_file);
}

void delete_rtable(trie_node_t *rtable) {
    delete_trie(rtable);
}

rtable_entry_t *search_rtable(trie_node_t *rtable, uint32_t address) {
    return search_prefix(rtable, address);
}
