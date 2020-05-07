#pragma once

#include <netinet/if_ether.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "list.h"
#include "queue.h"
#include "rtable.h"
#include "parser.h"

/*
 * IP and MAC address are stored in Host Byte Order.
 *
 */
typedef struct arp_entry {
    uint32_t ip;
    uint8_t mac[ETH_ALEN];
} arp_entry_t;

/*
 * Search for the matching MAC address for the IP address @ip
 * in @arp_table
 *
 */
uint8_t *arp_search_mac(list arp_table, uint32_t ip);

/*
 * Add entry in ARP table.
 *
 */
list arp_add_entry(arp_entry_t *entry, list arp_table);

/*
 * Send packets that wait in the pending queue and for which
 * the MAC destination address is known. Also pass rtable to this
 * function because we need to search for the interface in the
 * routing table.
 *
 */
void send_pending_packets(list arp_table, queue pending_q, trie_node_t *rtable);

/*
 * Free the memory of the arp_table and the memory of each
 * element from arp_table.
 *
 */
void delete_arp_table(list arp_table);
