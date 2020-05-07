#include "arp.h"
#include "queue.h"
#include "skel.h"
#include "protocols.h"
#include "checksum.h"

uint8_t *arp_search_mac(list arp_table, uint32_t ip) {
    list head = arp_table;

    while (head != NULL) {
        arp_entry_t *entry = (arp_entry_t *) head->element;

        if (entry->ip == ip) {
            return entry->mac;
        }

        head = head->next;
    }

    return NULL;
}

list arp_add_entry(arp_entry_t *entry, list arp_table) {
    return cons(entry, arp_table);
}

void send_pending_packets(list arp_table, queue pending_q, trie_node_t *rtable) {
    /*
     * Traverse the queue and send all packets for which the router
     * knows the matching ARP entry.
     *
     */

    if (!queue_empty(pending_q)) {
        size_t q_size = pending_q->size;

        for (size_t i = 0; i < q_size; i++) {
            packet *pending_p = queue_deq(pending_q);
            struct iphdr *pending_ip_hdr = (struct iphdr *) (pending_p->payload
                    + sizeof(struct ether_header));

            uint8_t *dest_mac = arp_search_mac(arp_table, ntohl(pending_ip_hdr->daddr));

            if (dest_mac == NULL) {
                puts("Packet is still in pending queue");
                queue_enq(pending_q, pending_p);

                continue;
            }

            puts("Packet waits to be forwarded after it was dequeued");

            /*
             * Look in the routing table again for the interface.
             *
             */
            rtable_entry_t *best_route = search_rtable(rtable, ntohl(pending_ip_hdr->daddr));
            if (best_route == NULL) {
                perror("send_pending_packet: best route was not refound");
                exit(EXIT_FAILURE);
            }

            int forward_packet_ret = forward_packet(best_route->interface,
                dest_mac, pending_p);

            if (forward_packet_ret) {
                puts("Packet was forwarded successfully");
            } else {
                puts("Packet could not be forwarded successfully");
            }

            // free the memory of the packet from pending queue
            free(pending_p);
        }

    } else {
        puts("Pending queue is empty");
    }

}


void delete_arp_table(list arp_table) {
    list head = arp_table;

    while (head != NULL) {
        free(head->element);
        head = cdr_and_free(head);
    }
}
