#include "skel.h"
#include "parser.h"
#include "protocols.h"
#include "checksum.h"
#include "list.h"
#include "arp.h"
#include "queue.h"

#include <net/if_arp.h>
#include <netinet/if_ether.h>

trie_node_t *rtable;
list arp_table;
queue pending_q;

int main(int argc, char *argv[])
{
	packet m;
	int rc;

	init();
    read_rtable(&rtable);
    pending_q = queue_create();

	while (1) {
        /*
         * Because get_packet is a blocking function, send the pending
         * packets before receiving a new packet.
         *
         */
        send_pending_packets(arp_table, pending_q, rtable);

		rc = get_packet(&m);
		DIE(rc < 0, "get_message");

        // Parse the message into appropriate headers.
        struct ether_header *eth_hdr = (struct ether_header *) m.payload;
        struct ether_arp *eth_arp_hdr = (struct ether_arp*) (m.payload + sizeof(struct ether_header));

        struct iphdr *ip_hdr = (struct iphdr *) (m.payload + sizeof(struct ether_header));
        struct icmphdr *icmp_hdr = (struct icmphdr *) (m.payload + sizeof(struct ether_header)
                        + sizeof(struct iphdr));

        /* Get the ip address of the interface on which the packet was sent. */
        /* Must be converted to int type using inet_addr. */
        char *if_ip_address = get_interface_ip(m.interface);

        /*
         *
         * If the router receives an ARP request from a host it means that the
         * router must send back its MAC address in an ARP reply.
         *
         */

        /* Convert an array of 4 uint8_t into a uint32_t. */
        uint32_t eth_arp_target_ip_addr = *((uint32_t *)&eth_arp_hdr->arp_tpa);

        if ((ntohs(eth_hdr->ether_type) == ETH_P_ARP) &&
            (ntohs(eth_arp_hdr->ea_hdr.ar_op) == ARPOP_REQUEST) &&
            (inet_addr(if_ip_address) == eth_arp_target_ip_addr)) {

            puts("Router received ARP request");

            /*
             *
             * Send a packet on the same interface to the host that required router's
             * MAC address.
             *
             */
            int send_ARP_reply_ret = send_ARP_reply(m.interface,
                    eth_arp_hdr->arp_sha, eth_arp_hdr->arp_spa);

            if (send_ARP_reply_ret) {
                puts("Router sent back ARP reply successfully");
            } else {
                puts("Router could not send back ARP reply successfully");
            }

            continue;
        }

        /*
         *  If the router receives an ICMP request then it must respond with an
         *  ICMP reply.
         *
         */
        if ((ntohs(eth_hdr->ether_type) == ETH_P_IP) &&
            (ip_hdr->protocol == IPPROTO_ICMP) && (icmp_hdr->type == ICMP_ECHO)
            && (inet_addr(if_ip_address) == ip_hdr->daddr)) {

            puts("Router received ICMP request");

            /*
             * Send a packet on the same interface to the host that sent a ICMP
             * request.
             *
             */

            /*
             * The payload used in request must be the same with the one in reply.
             * Copy tot_len - sizeof struct iphdr - sizeof struct icmphdr bytes
             * from icmp_hdr + sizeof icmphdr and paste them in reply.
             *
             */
            uint8_t *payload = (uint8_t *) (m.payload + sizeof(struct ether_header)
                + sizeof(struct iphdr) + sizeof(struct icmphdr));
            uint32_t payload_sz = ntohs(ip_hdr->tot_len) - sizeof(struct icmphdr) -
                    sizeof(struct iphdr);

            int send_ICMP_reply_ret = send_ICMP_reply(m.interface,
                eth_hdr->ether_shost, ip_hdr->saddr, icmp_hdr->un.echo.id,
                icmp_hdr->un.echo.sequence, payload, payload_sz);

            if (send_ICMP_reply_ret) {
                puts("Router sent back ICMP reply successfully");
            } else {
                puts("Router could not send back ICMP reply successfully");
            }

            continue;
        }

        /*
         * Check the TTL of the packet, if TTL <= 1 then send ICMP Time Exceeded Message.
         *
         */
         if (ip_hdr->ttl <= 1) {
            puts("Router received a packet with invalid TTL");

            // tem_payload is a pointer to the start of the sender's IP header
            uint8_t *tem_payload = (uint8_t *) (m.payload + sizeof(struct ether_header));
            uint32_t tem_payload_sz = ntohs(ip_hdr->tot_len);

            int send_ICMP_tem_ret = send_ICMP_tem(m.interface, eth_hdr->ether_shost,
                ip_hdr->saddr, tem_payload, tem_payload_sz);

            if (send_ICMP_tem_ret) {
                puts("Router sent back ICMP TEM successfully");
            } else {
                puts("Router could not send back ICMP TEM successfully");
            }

            continue;
        }

        /*
         *  If the router receives an ARP reply then update the ARP table and
         *  send the pending messages from queue in the next iteration of the
         *  while loop.
         *
         */
        if ((ntohs(eth_hdr->ether_type) == ETH_P_ARP) &&
            (ntohs(eth_arp_hdr->ea_hdr.ar_op) == ARPOP_REPLY) &&
            (inet_addr(if_ip_address) == eth_arp_target_ip_addr)) {

            puts("Router received ARP reply");

            /*
             * Add the new received MAC address in the ARP table
             * alongside with its IP address.
             *
             */
            arp_entry_t *entry = calloc(1, sizeof(arp_entry_t));

            int *n_ip = (int *) eth_arp_hdr->arp_spa;
            entry->ip = ntohl(*n_ip);
            memcpy(entry->mac, eth_arp_hdr->arp_sha, ETH_ALEN);

            arp_table = cons(entry, arp_table);

            puts("ARP table updated");

            continue;
        }

        /*
         * Verify the checksum and drop the packet if the checksum is incorrect.
         *
         */

        // checksum that is received in packet
        uint16_t ip_hdr_check = ip_hdr->check;
        ip_hdr->check = 0;

        // compare the received checksum with the calculated one
        if (checksum(ip_hdr, sizeof(struct iphdr)) != ip_hdr_check) {
            puts("Incorrect checksum, dropping the packet");
            continue;
        }

        // restore the checksum
        ip_hdr->check = ip_hdr_check;

        /*
         * Search in the routing table for the best route. If search_rtable cannot
         * find a matching address then send ICMP Destinaton Unreachable Message.
         *
         */
        rtable_entry_t *best_route = search_rtable(rtable, ntohl(ip_hdr->daddr));
        if (best_route == NULL) {
            puts("Destination unreachable, sending ICMP DUM");

            // dum_payload is a pointer to the start of the sender's IP header
            uint8_t *dum_payload = (uint8_t *) (m.payload + sizeof(struct ether_header));
            uint32_t dum_payload_sz = ntohs(ip_hdr->tot_len);

            int send_ICMP_dum_ret = send_ICMP_dum(m.interface, eth_hdr->ether_shost,
                ip_hdr->saddr, dum_payload, dum_payload_sz);

            if (send_ICMP_dum_ret) {
                puts("Router sent back ICMP DUM successfully");
            } else {
                puts("Router could not send back ICMP DUM successfully");
            }

            continue;
        }

        /*
         * Decrement TTL and update the checksum.
         *
         */
        ip_hdr->ttl--;

        ip_hdr->check = 0;
        ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

        /*
         * Search in ARP table for a matching entry for next_hop. If no entry
         * is found the send ARP request to next_hop and put the packet in the
         * queue.
         *
         */

        uint8_t *dest_mac = arp_search_mac(arp_table, ntohl(best_route->next_hop));
        if (dest_mac == NULL) {
            puts("Matching MAC address not found in ARP table, generating ARP request");

            // convert next_hop to network byte order and put the result in an array of
            // 4 uint8_t's
            uint32_t n_ip_dest = best_route->next_hop;
            uint8_t *ip_dest = (uint8_t *) &n_ip_dest;

            int send_ARP_request_ret = send_ARP_request(best_route->interface, ip_dest);

            if (send_ARP_request_ret) {
                puts("Router sent ARP request successfully");
            } else {
                puts("Router could not send ARP request successfully");
            }

            // add packet in queue
            packet *p = calloc(1, sizeof(packet));
            memcpy(p, &m, sizeof(packet));

            queue_enq(pending_q, p);
            puts("Packet was inserted in pending queue");

            continue;
        } else {
            int forward_packet_ret = forward_packet(best_route->interface, dest_mac, &m);

            if (forward_packet_ret) {
                puts("Packet was forwarded successfully");
            } else {
                puts("Packet could not be forwarded successfully");
            }
        }

	}

    /*
     * Delete the memory on the heap.
     *
     */
    delete_rtable(rtable);
    rtable = NULL;

    delete_arp_table(arp_table);
    arp_table = NULL;

    delete_queue(pending_q);
    pending_q = NULL;

    return EXIT_SUCCESS;
}
