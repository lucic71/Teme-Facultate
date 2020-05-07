#pragma once

#include <stdint.h>
#include <unistd.h>

/*
 * Computes IP checksum of a block @vdata of size @length
 * using the incremental algorithm from RFC 1624.
 *
 */
uint16_t checksum(void *vdata, size_t length);
