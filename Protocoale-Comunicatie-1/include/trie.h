#pragma once

#include <stdlib.h>

#include "rtable.h"

/*
 * Structure that defines a binary trie with the following traversal
 * rules: 1 for right and 0 for left.
 *
 * At each end of prefix that is inserted in the trie there is a reference
 * to a rtable_entry_t that keeps track on the longest matching prefix.
 * The rest of entry pointers are NULL.
 *
 */
typedef struct trie_node {
    struct trie_node *right, *left;
    rtable_entry_t *entry;
} trie_node_t;

/*
 * Returns a new node to be inserted in trie.
 *
 */
trie_node_t *create_node();

/*
 * @root: root of the trie
 * @address: address to be inserted
 * @length: length of address (do not insert all 32 bits, instead perform bitwise
 *  and between prefix and mask and insert only first @length set bits from mask)
 * @entry: pointer to the corresponding pointer for address
 *
 */
void insert_node(trie_node_t *root, uint32_t address, int length, rtable_entry_t *entry);

/*
 * @root: root of the trie
 * @address: address that will traverse the trie until it finds an
 *  rtable_entry_t or returns NULL
 *
 */
rtable_entry_t *search_prefix(trie_node_t *root, uint32_t address);

/*
 * @root: root of trie to be deleted
 *
 */
void delete_trie(trie_node_t *root);

