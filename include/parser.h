#pragma once
#include "trie.h"
#include "rtable.h"

/*
 * @rtable: double pointer to the new allocated memory zone that keeps a routing
 * table reed from rtable.txt
 *
 */
void read_rtable(trie_node_t **rtable);

/*
 * search in @rtable for the appropriate next hop for @address using
 * the longest prefix match rule.
 *
 * IMPORTANT!
 *  Address must be in Host Byte Order, use ntohl function to convert if
 *  necessary.
 *
 * @return: a rtable_entry_t for the next hop on success
 *          NULL on failure
 *
 */
rtable_entry_t *search_rtable(trie_node_t *rtable, uint32_t address);

/*
 * @rtable: rtable to be deleted
 *
 */
void delete_rtable(trie_node_t *rtable);
