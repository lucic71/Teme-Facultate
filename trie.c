#include <stdio.h>
#include <string.h>

#include "trie.h"

#define INT_BITS 32

trie_node_t *create_node() {
    return calloc(1, sizeof(trie_node_t));
}

uint32_t left_rotate(uint32_t n, unsigned int d) {
    return (n << d)|(n >> (INT_BITS - d));
}


void insert_node(trie_node_t *root, uint32_t address, int length, rtable_entry_t *entry) {

    if (root == NULL) {
        perror("insert_node: null root");
        exit(EXIT_FAILURE);
    }

    if (entry == NULL) {
        perror("insert_node: null entry");
        exit(EXIT_FAILURE);
    }

    int level;
    trie_node_t *crawl = root;

    for (level = 1; level <= length; level++) {
        int bit = (left_rotate(address, level) & 0x00000001);

        if (bit == 1) {
            if (crawl->right == NULL) {
                crawl->right = create_node();
            }

            crawl = crawl->right;
        } else if(bit == 0) {
            if (crawl->left == NULL) {
                crawl->left = create_node();
            }

            crawl = crawl->left;
        }
    }

    crawl->entry = calloc(1, sizeof(rtable_entry_t));

    crawl->entry->prefix = entry->prefix;
    crawl->entry->next_hop = entry->next_hop;
    crawl->entry->mask = entry->mask;
    crawl->entry->interface = entry->interface;

}

rtable_entry_t *search_prefix(trie_node_t *root, uint32_t address) {

    if (root == NULL) {
        perror("insert_node: null root");
        exit(EXIT_FAILURE);
    }

    rtable_entry_t *entry = NULL;

    int level;
    int length = INT_BITS;
    trie_node_t *crawl = root;

    for (level = 1; level <= length; level++) {
        int bit = (left_rotate(address, level) & 0x00000001);

        if (bit == 1) {
            crawl = crawl->right;
        } else {
            crawl = crawl->left;
        }

        if (crawl != NULL) {
            entry = crawl->entry;
        } else {
            return entry;
        }
    }

    return NULL;
}

void delete_trie(trie_node_t *root) {
    if (root == NULL) {
        return;
    }

    delete_trie(root->left);
    delete_trie(root->right);

    free(root->entry);
    root->entry = NULL;

    free(root);
    root = NULL;
}
