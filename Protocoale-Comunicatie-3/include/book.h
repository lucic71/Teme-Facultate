#ifndef BOOK_H_
#define BOOK_H_

#include "cJSON.h"

/*
 * Number of elements in a book_info structure.
 *
 */
#define BOOK_INFO_ELEMS 5

/*
 * Max number of characters in the strings defined in book_info_t.
 *
 */
#define MAX_BOOK_ELEM_SIZE    100
#define MAX_BOOK_ID_ELEM_SIZE 100

/*
 * Names of prompts in ADD_BOOK operation.
 *
 */
#define ID         "id"

#define TITLE      "title"
#define AUTHOR     "author"
#define GENRE      "genre"
#define PUBLISHER  "publisher"
#define PAGE_COUNT "page_count"

typedef struct book_id {
    char *id;
} book_id_t;


typedef struct book_info {
    char *title;
    char *author;
    char *genre;
    int  *page_count;
    char *publisher;
} book_info_t;

/*
 * Display a prompt for each element in book_info_t, let the user enter the
 * info and return it in a book_info_t data type.
 *
 */
book_info_t *get_book_info();

/*
 * Display a prompt for each element in book_id_t, let the user enter the
 * info and return it in a book_id_t data type.
 *
 */
book_id_t *get_book_id();

/*
 * Convert the book_info_t object to JSON format.
 *
 */
cJSON *book_to_json(book_info_t *book_info);

/*
 * Frees the memory for a book_info_t object.
 *
 */
void delete_book_info(book_info_t *book_info);

/*
 * Frees the memory for a book_id_t object.
 *
 */
void delete_book_id(book_id_t *book_id);

/*
 * Print the ID and the TITLE of each book received in @book_list_string
 *
 */
void print_json_book_list(char *book_list_string);

/*
 * Print a JSON book accordingly to book_info_t and book_id_t format.
 *
 */
void print_json_book(char *book_string);


#endif
