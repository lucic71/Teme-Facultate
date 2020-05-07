I. File list
------------
router.c    main file containg the functionality of the router
arp.c       contains function that handle the ARP table
checksum.c  implementation of checksum algorithm RFC 1624
list.c      interface for using a list
parser.c    contains functions that handle the routing table
protocols.c implementation of ICMP, ARP protocols and forwarding
queue.c     interface for using a queue
skel.c      skeleth provided with different helpers
trie.c      interface for using a trie
include/    header files for the above mentioned source files
Makefile    Makefile to build router
README      this file

II. Design
----------
A. Program design
First thing the router does in main, after init() is called, is to parse the
routing table and to create a pending queue for the packets that cannot be
immediately sent to the destination.
In the while loop the router ensures that all pending packets are sent to their
destination by calling send_pending_packets() at line 31. After that in receives
a packet @m and parses it into the appropiate headers(Ether, IP, ICMP or ARP).
The next thing the router does is to check what kind of packet @m is. The
steps are:
    - check if the packet is an ARP request, if so send ARP reply
    - check if the packet is an ICMP request, if so send ICMP reply
    - check if packet has TTL <= 1, if so send ICMP Time Exceeded Message
    - check if packet is ARP reply, if so update ARP table
    - check if packet has a valid checksum, if not drop the packet
    - search in the routing table for next hop, if not found then send ICMP
        Destination Unreachable Message
    - decrement TTL and update checksum
    - search in ARP table for MAC address of next hop. if not found then send
        an ARP request, else forward the packet
All this functionality is checked using if clauses and performed using
functions from protocols.h.
Also relevant messages are displayed to make debugging easier.

B. Routing table and Trie
For representing the routing table in an efficient way the program uses a trie
data structure defined in trie.h. The best part about this structure is that
it has a searching time of O(1), because the length of the words inserted in
the trie is in the worst case 32, which is constant. The memory requirements
are also pretty good, the space being O(N), where N is the number of keys
inserted.

C. Pending queue and ARP table
The program uses list to represent the two types. Keeping in mind that the cons
operation is O(1) their representation is also very efficient. The only
bottleneck is that the pending queue traversal uses the dequeue and enqueue
operations to check what packets can be forwarded and which stay in queue.
This is the result of the fact that the router cannot be sure that the first
packet in queue is the right candidate to be forwarded due to the way request
and replies are transmited in the network.

III. Protocols
--------------
As mentioned above, the protocols implemented in this homework are IP, ICMP,
ARP and Ethernet. They can be all found in protocols.c.

A. IP
This protocol is used in all functions instead of ARP. In my opinion it was
the most complicated one because it has a lot of fields and it is very hard
to find out what is the exact meaning of each one.

B. ICMP
The router generates ICMP replies, time exceeded messages and destination
unreachable messages. They are used mainly to colect information about
the hosts. The tricky part here was to realize that the payload of this
protocol must include information about the packet to which it refers.

C. ARP
The router can generate ARP request and replies by creating a packet containing
a struct ether_header and a struct ether_arp header. The only difference between
a request and a reply is the ar_op field from ether_arp->ea_hdr->arp_op.
Reply: ARPOP_REPLY, Request: ARPOP_REQUEST.

D. Ethernet
This protocol is pretty simple, its header contains a destination MAC address,
a source MAC address and a protocol type. The connection is point-to-point
so the hops must each change the dhost and shost fields when they receive
an Ether packet.

IV. Results and conclusions
---------------------------
On my machine the only thing to mention is that the first packet transmitted
when ping-ing a host take slightly more time to process, maybe because in
update the ARP table in the current iteration and just in the next iteration
the router processes the pending packets. A custom output looks like following:
64 bytes from h1 (192.168.1.2): icmp_seq=1 ttl=63 time=0.453 ms
64 bytes from h1 (192.168.1.2): icmp_seq=2 ttl=63 time=0.256 ms
64 bytes from h1 (192.168.1.2): icmp_seq=3 ttl=63 time=0.239 ms
64 bytes from h1 (192.168.1.2): icmp_seq=4 ttl=63 time=0.249 ms

Anyway empirically, the first packet does not take more than 0.5 ms to be
processed.

As a final note, the only thing to be mentioned is that because the router is
not shut down correctly most of the time, there will be memory leaks, which
are, generally speaking, a bad thing. A workaround for this would be a signal
handler that catches the CTRL-C or CTRL-Z signals and frees the memory
associated with the routing table, pending queue and ARP table.
