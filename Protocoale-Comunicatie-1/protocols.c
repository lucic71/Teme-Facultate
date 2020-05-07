#include "protocols.h"
#include "skel.h"
#include "checksum.h"

#include <netinet/if_ether.h>

// used in send_ARP_reply
#define IP_ALEN 4

int send_ARP_reply(int interface, uint8_t *dest_mac, uint8_t *ip_dest) {
    packet s_arp;

    // already in network byte order
    char *source_ip_char = get_interface_ip(interface);
    uint32_t source_ip_int = inet_addr(source_ip_char);

    uint8_t *source_mac = calloc(1, ETH_ALEN);
    get_interface_mac(interface, source_mac);

    /* Craft ETHER header. */
    struct ether_header *eth_hdr = (struct ether_header *) s_arp.payload;

    // ether_dhost
    memcpy(&eth_hdr->ether_dhost, dest_mac, ETH_ALEN);

    // ether_shost
    memcpy(&eth_hdr->ether_shost, source_mac, ETH_ALEN);

    // ether_type
    eth_hdr->ether_type = htons(ETH_P_ARP);

    /* Craft ETHER ARP header. */
    struct ether_arp *eth_arp_hdr = (struct ether_arp *) (s_arp.payload
            + sizeof(struct ether_header));

    // struct arphdr
    eth_arp_hdr->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    eth_arp_hdr->ea_hdr.ar_pro = htons(0x0800);
    eth_arp_hdr->ea_hdr.ar_hln = ETH_ALEN;
    eth_arp_hdr->ea_hdr.ar_pln = IP_ALEN;
    eth_arp_hdr->ea_hdr.ar_op  = htons(ARPOP_REPLY);

    // source hardware and protocol address
    memcpy(&eth_arp_hdr->arp_sha, source_mac, ETH_ALEN);
    memcpy(&eth_arp_hdr->arp_spa, &source_ip_int, IP_ALEN);

    // target hardware and protocol address
    memcpy(&eth_arp_hdr->arp_tha, dest_mac, ETH_ALEN);
    memcpy(&eth_arp_hdr->arp_tpa, ip_dest, IP_ALEN);

    s_arp.interface = interface;
    s_arp.len = sizeof(struct ether_header) + sizeof(struct ether_arp);

    // free the memory used by source_mac
    free(source_mac);

    int ret_val = send_packet(interface, &s_arp);
    return ret_val;
}

int send_ARP_request(int interface, uint8_t *ip_dest) {
    packet s_arp;

    // already in network byte order
    char *source_ip_char = get_interface_ip(interface);
    uint32_t source_ip_int = inet_addr(source_ip_char);

    uint8_t *source_mac = calloc(1, ETH_ALEN);
    get_interface_mac(interface, source_mac);

    /* Craft ETHER header. */
    struct ether_header *eth_hdr = (struct ether_header *) s_arp.payload;

    // ether_dhost -- broadcast
    memset(&eth_hdr->ether_dhost, 0xff, ETH_ALEN);

    // ether_shost
    memcpy(&eth_hdr->ether_shost, source_mac, ETH_ALEN);

    // ether_type
    eth_hdr->ether_type = htons(ETH_P_ARP);

    /* Craft ETHER ARP header. */
    struct ether_arp *eth_arp_hdr = (struct ether_arp *) (s_arp.payload
            + sizeof(struct ether_header));

    // struct arphdr
    eth_arp_hdr->ea_hdr.ar_hrd = htons(ARPHRD_ETHER);
    eth_arp_hdr->ea_hdr.ar_pro = htons(0x0800);
    eth_arp_hdr->ea_hdr.ar_hln = ETH_ALEN;
    eth_arp_hdr->ea_hdr.ar_pln = IP_ALEN;
    eth_arp_hdr->ea_hdr.ar_op  = htons(ARPOP_REQUEST);

    // source hardware and protocol address
    memcpy(&eth_arp_hdr->arp_sha, source_mac, ETH_ALEN);
    memcpy(&eth_arp_hdr->arp_spa, &source_ip_int, IP_ALEN);

    // target hardware and protocol address -- broadcast
    memset(&eth_arp_hdr->arp_tha, 0xff, ETH_ALEN);
    memcpy(&eth_arp_hdr->arp_tpa, ip_dest, IP_ALEN);

    s_arp.interface = interface;
    s_arp.len = sizeof(struct ether_header) + sizeof(struct ether_arp);

    // free the memory used by source_mac
    free(source_mac);

    int ret_val = send_packet(interface, &s_arp);
    return ret_val;
}

int send_ICMP_reply(int interface, uint8_t *dest_mac, uint32_t ip_dest,
                    int id, int seq, uint8_t *payload, uint32_t payload_sz) {
    packet s_icmp;

    // already in network byte order
    char *source_ip_char = get_interface_ip(interface);
    uint32_t source_ip_int = inet_addr(source_ip_char);

    uint8_t *source_mac = calloc(1, ETH_ALEN);
    get_interface_mac(interface, source_mac);

    // ETHER HEADER
    struct ether_header *eth_hdr = (struct ether_header *) s_icmp.payload;

    // ether_dhost
    memcpy(&eth_hdr->ether_dhost, dest_mac, ETH_ALEN);

    // ether_shost
    memcpy(&eth_hdr->ether_shost, source_mac, ETH_ALEN);

    // ether_type
    eth_hdr->ether_type = htons(ETH_P_IP);

    // IP HEADER
    struct iphdr *ip_hdr = (struct iphdr *) (s_icmp.payload
        + sizeof(struct ether_header));

    ip_hdr->version  = IPVERSION;
    ip_hdr->ihl      = 0x05;

    ip_hdr->tos      = 0x00;
    ip_hdr->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct icmphdr)
            + payload_sz);
    ip_hdr->id       = 0x00;
    ip_hdr->frag_off = 0x00;
    ip_hdr->ttl = 64;
    ip_hdr->protocol = IPPROTO_ICMP;

    ip_hdr->saddr = source_ip_int;
    ip_hdr->daddr = ip_dest;

    ip_hdr->check = 0;
    ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

    // ICMP HEADER
    struct icmphdr *icmp_hdr = (struct icmphdr *) (s_icmp.payload
        + sizeof(struct ether_header) + sizeof(struct iphdr));

    icmp_hdr->type = ICMP_ECHOREPLY;
    icmp_hdr->code = 0x00;

    icmp_hdr->un.echo.id       = id;
    icmp_hdr->un.echo.sequence = seq;

    // PAYLOAD
    uint8_t *injected_payload = (uint8_t *) (s_icmp.payload
        + sizeof(struct ether_header) + sizeof(struct iphdr)
        + sizeof(struct icmphdr));


    memcpy(injected_payload, payload, payload_sz);

    // ICMP HEADER CHECKSUM
    icmp_hdr->checksum = 0;
    icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr)
        + payload_sz);

    s_icmp.interface = interface;
    s_icmp.len = sizeof(struct ether_header) + sizeof(struct iphdr)
        + sizeof(struct icmphdr) + payload_sz;

    // free the memory used by source_mac
    free(source_mac);

    int ret_val = send_packet(interface, &s_icmp);
    return ret_val;
}

int send_ICMP_tem(int interface, uint8_t *dest_mac, uint32_t ip_dest,
                    uint8_t *tem_payload, uint32_t tem_payload_sz) {

    packet s_icmp_tem;

    // already in network byte order
    char *source_ip_char = get_interface_ip(interface);
    uint32_t source_ip_int = inet_addr(source_ip_char);

    uint8_t *source_mac = calloc(1, ETH_ALEN);
    get_interface_mac(interface, source_mac);

    // ETHER HEADER
    struct ether_header *eth_hdr = (struct ether_header *) s_icmp_tem.payload;

    // ether_dhost
    memcpy(&eth_hdr->ether_dhost, dest_mac, ETH_ALEN);

    // ether_shost
    memcpy(&eth_hdr->ether_shost, source_mac, ETH_ALEN);

    // ether_type
    eth_hdr->ether_type = htons(ETH_P_IP);

    // IP HEADER
    struct iphdr *ip_hdr = (struct iphdr *) (s_icmp_tem.payload
        + sizeof(struct ether_header));

    ip_hdr->version  = IPVERSION;
    ip_hdr->ihl      = 0x05;

    ip_hdr->tos      = 0x00;
    ip_hdr->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct icmphdr)
            + tem_payload_sz);
    ip_hdr->id       = 0x00;
    ip_hdr->frag_off = 0x00;
    ip_hdr->ttl = 64;
    ip_hdr->protocol = IPPROTO_ICMP;

    ip_hdr->saddr = source_ip_int;
    ip_hdr->daddr = ip_dest;

    ip_hdr->check = 0;
    ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

    // ICMP HEADER
    struct icmphdr *icmp_hdr = (struct icmphdr *) (s_icmp_tem.payload
        + sizeof(struct ether_header) + sizeof(struct iphdr));

    icmp_hdr->type = ICMP_TIME_EXCEEDED;
    icmp_hdr->code = 0x00;

    icmp_hdr->un.echo.id       = 0x00;
    icmp_hdr->un.echo.sequence = 0x00;

    // PAYLOAD
    uint8_t *injected_payload = (uint8_t *) (s_icmp_tem.payload
        + sizeof(struct ether_header) + sizeof(struct iphdr)
        + sizeof(struct icmphdr));


    memcpy(injected_payload, tem_payload, tem_payload_sz);

    // ICMP HEADER CHECKSUM
    icmp_hdr->checksum = 0;
    icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr)
        + tem_payload_sz);

    s_icmp_tem.interface = interface;
    s_icmp_tem.len = sizeof(struct ether_header) + sizeof(struct iphdr)
        + sizeof(struct icmphdr) + tem_payload_sz;

    // free the memory used by source_mac
    free(source_mac);

    int ret_val = send_packet(interface, &s_icmp_tem);
    return ret_val;
}

int send_ICMP_dum(int interface, uint8_t *dest_mac, uint32_t ip_dest,
                    uint8_t *dum_payload, uint32_t dum_payload_sz) {

    packet s_icmp_dum;

    // already in network byte order
    char *source_ip_char = get_interface_ip(interface);
    uint32_t source_ip_int = inet_addr(source_ip_char);

    uint8_t *source_mac = calloc(1, ETH_ALEN);
    get_interface_mac(interface, source_mac);

    // ETHER HEADER
    struct ether_header *eth_hdr = (struct ether_header *) s_icmp_dum.payload;

    // ether_dhost
    memcpy(&eth_hdr->ether_dhost, dest_mac, ETH_ALEN);

    // ether_shost
    memcpy(&eth_hdr->ether_shost, source_mac, ETH_ALEN);

    // ether_type
    eth_hdr->ether_type = htons(ETH_P_IP);

    // IP HEADER
    struct iphdr *ip_hdr = (struct iphdr *) (s_icmp_dum.payload
        + sizeof(struct ether_header));

    ip_hdr->version  = IPVERSION;
    ip_hdr->ihl      = 0x05;

    ip_hdr->tos      = 0x00;
    ip_hdr->tot_len  = htons(sizeof(struct iphdr) + sizeof(struct icmphdr)
            + dum_payload_sz);
    ip_hdr->id       = 0x00;
    ip_hdr->frag_off = 0x00;
    ip_hdr->ttl = 64;
    ip_hdr->protocol = IPPROTO_ICMP;

    ip_hdr->saddr = source_ip_int;
    ip_hdr->daddr = ip_dest;

    ip_hdr->check = 0;
    ip_hdr->check = checksum(ip_hdr, sizeof(struct iphdr));

    // ICMP HEADER
    struct icmphdr *icmp_hdr = (struct icmphdr *) (s_icmp_dum.payload
        + sizeof(struct ether_header) + sizeof(struct iphdr));

    icmp_hdr->type = ICMP_DEST_UNREACH;
    icmp_hdr->code = 0x00;

    icmp_hdr->un.echo.id       = 0x00;
    icmp_hdr->un.echo.sequence = 0x00;

    // PAYLOAD
    uint8_t *injected_payload = (uint8_t *) (s_icmp_dum.payload
        + sizeof(struct ether_header) + sizeof(struct iphdr)
        + sizeof(struct icmphdr));


    memcpy(injected_payload, dum_payload, dum_payload_sz);

    // ICMP HEADER CHECKSUM
    icmp_hdr->checksum = 0;
    icmp_hdr->checksum = checksum(icmp_hdr, sizeof(struct icmphdr)
        + dum_payload_sz);

    s_icmp_dum.interface = interface;
    s_icmp_dum.len = sizeof(struct ether_header) + sizeof(struct iphdr)
        + sizeof(struct icmphdr) + dum_payload_sz;

    // free the memory used by source_mac
    free(source_mac);

    int ret_val = send_packet(interface, &s_icmp_dum);
    return ret_val;
}

int forward_packet(int interface, uint8_t *dest_mac, packet *p) {
    struct ether_header *eth_hdr = (struct ether_header *) p->payload;
    memcpy(&eth_hdr->ether_dhost, dest_mac, ETH_ALEN);

    int ret_val = send_packet(interface, p);
    return ret_val;
}
