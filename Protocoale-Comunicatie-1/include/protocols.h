#pragma once

#include <stdint.h>

#include "skel.h"

/*
 * IP and MAC addresses are passed in Network Byte Order.
 *
 */

/*
 * @interface is used to get the ip address and mac address of host
 * @dest_mac  is the mac address of destination
 * @ip_dest   is the ip address of destination
 *
 * @returns: 0 on success
 *          -1 on error
 *
 */
int send_ARP_reply(int interface, uint8_t *dest_mac, uint8_t *ip_dest);

int send_ARP_request(int interface, uint8_t *ip_dest);

/*
 * @id and @seq are parameters for icmp header
 *  they are used to identify a reply for a certain request
 *
 * @payload: the receiver must send the same payload used by the sender
 *  in order to keep track on the timestamp and other information
 *
 * @payload_sz: size of payload
 *
 */
int send_ICMP_reply(int interface, uint8_t *dest_mac, uint32_t ip_dest,
                    int id, int seq, uint8_t *payload, uint32_t payload_sz);

/*
 * @tem_payload: ICMP TEM(Time Exceeded Message) must send back in as
 *  payload the sender's IP and ICMP header, represented here by
 *  tem_payload
 *
 * @tem_payload_sz: size of tem_payload
 *
 */
int send_ICMP_tem(int interface, uint8_t *dest_mac, uint32_t ip_dest,
                    uint8_t *tem_payload, uint32_t tem_payload_sz);

/*
 * @dum_payload and @dum_payload_sz work exactly the same as @tem_payload
 * and @tem_payload_sz in #send_ICMP_tem
 *
 * dum is an acronym for Destination Unreachable Message.
 *
 */
int send_ICMP_dum(int interface, uint8_t *dest_mac, uint32_t ip_dest,
                    uint8_t *dum_payload, uint32_t dum_payload_sz);

/*
 * Send packet @p on interface @interface using as destination
 * MAC @dest_mac.
 *
 */
int forward_packet(int interface, uint8_t *dest_mac, packet *p);
