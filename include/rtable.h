#pragma once

#include <stdint.h>
#include <unistd.h>

/*
 * Addresses are stored in Network Byte Order.
 *
 */
struct route_table_entry {
	uint32_t prefix;
	uint32_t next_hop;
	uint32_t mask;
	int interface;
}__attribute__((packed));

typedef struct route_table_entry rtable_entry_t;
