#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <book.h>
#include <error.h>
#include <memory.h>
#include <prompt.h>
#include <cJSON.h>

book_info_t *get_book_info() {
    /*
     * Read the elements from a book.
     *
     */
    char *title      = read_item(TITLE, MAX_BOOK_ELEM_SIZE, ECHO_ON);
    char *author     = read_item(AUTHOR, MAX_BOOK_ELEM_SIZE, ECHO_ON);
    char *genre      = read_item(GENRE, MAX_BOOK_ELEM_SIZE, ECHO_ON);
    char *page_count = read_item(PAGE_COUNT, MAX_BOOK_ELEM_SIZE, ECHO_ON);
    char *publisher  = read_item(PUBLISHER, MAX_BOOK_ELEM_SIZE, ECHO_ON);

    NONVOID_ERROR_HANDLER(title == NULL, "[ERROR] Could not read title", NULL);
    NONVOID_ERROR_HANDLER(author == NULL, "[ERROR] Could not read author", NULL);
    NONVOID_ERROR_HANDLER(genre == NULL, "[ERROR] Could not read genre", NULL);
    NONVOID_ERROR_HANDLER(page_count == NULL, "[ERROR] Could not read page_count", NULL);
    NONVOID_ERROR_HANDLER(publisher == NULL, "[ERROR] Could not read publisher", NULL);

    /*
     * Convert page_count to int.
     *
     */
    int *page_count_integer;
    CALLOC(page_count_integer, sizeof(int));

    sscanf(page_count, "%d", page_count_integer);

    /*
     * Build the book_info_t data type.
     *
     */
    book_info_t *book_info;
    CALLOC(book_info, sizeof(book_info));

    book_info->title      = title;
    book_info->author     = author;
    book_info->genre      = genre;
    book_info->page_count = page_count_integer;
    book_info->publisher  = publisher;

    return book_info;
}

cJSON *book_to_json(book_info_t *book_info) {

    cJSON *json_object = cJSON_CreateObject();

    cJSON *title_item = cJSON_CreateString(book_info->title);
    cJSON_AddItemToObject(json_object, TITLE, title_item);

    cJSON *author_item = cJSON_CreateString(book_info->author);
    cJSON_AddItemToObject(json_object, AUTHOR, author_item);

    cJSON *genre_item = cJSON_CreateString(book_info->genre);
    cJSON_AddItemToObject(json_object, GENRE, genre_item);

    cJSON *page_count_item = cJSON_CreateNumber(*(book_info->page_count));
    cJSON_AddItemToObject(json_object, PAGE_COUNT, page_count_item);

    cJSON *publisher_item = cJSON_CreateString(book_info->publisher);
    cJSON_AddItemToObject(json_object, PUBLISHER, publisher_item);

    return json_object;

}



void delete_book_info(book_info_t *book_info) {

    FREE(book_info->title);
    FREE(book_info->author);
    FREE(book_info->genre);
    FREE(book_info->page_count);
    FREE(book_info->publisher);

    FREE(book_info);

}
