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

    if (title == NULL || author == NULL || genre == NULL || page_count == NULL
            || publisher == NULL) {

        FREE(title);
        FREE(author);
        FREE(genre);
        FREE(page_count);
        FREE(publisher);

        return NULL;

    }

    /*
     * Convert page_count to int and delete page_count.
     *
     */
    int *page_count_integer;
    CALLOC(page_count_integer, sizeof(int));

    sscanf(page_count, "%d", page_count_integer);
    FREE(page_count);

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

book_id_t *get_book_id() {

    char *id = read_item(ID, MAX_BOOK_ID_ELEM_SIZE, ECHO_ON);

    if (id == NULL) {
        FREE(id);

        return NULL;
    }

    /*
     * Build the book_id_t data type.
     *
     */
    book_id_t *book_id;
    CALLOC(book_id, sizeof(book_id));

    book_id->id = id;
    return book_id;

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

    if (book_info == NULL) {
        return;
    }

    FREE(book_info->title);
    FREE(book_info->author);
    FREE(book_info->genre);
    FREE(book_info->page_count);
    FREE(book_info->publisher);

    FREE(book_info);

}

void delete_book_id(book_id_t *book_id) {

    if (book_id == NULL) {
        return;
    }

    FREE(book_id->id);
    FREE(book_id);

}

void print_json_book_list(char *book_list_string) {

    int books_counter = 0;

    cJSON *book_list = cJSON_Parse(book_list_string);
    cJSON *book      = NULL;

    cJSON_ArrayForEach(book, book_list) {

        cJSON *title  = cJSON_GetObjectItemCaseSensitive(book, TITLE);
        cJSON *id     = cJSON_GetObjectItemCaseSensitive(book, ID);

        char *title_string = cJSON_Print(title);
        char *id_string = cJSON_Print(id);

        printf("\n%s: %s\n"
               "%s: %s\n\n",
               TITLE, title_string,
               ID,    id_string);

        FREE(title_string);
        FREE(id_string);

        books_counter++;
    }

    cJSON_Delete(book_list);

    if (books_counter == 0) {
        puts(NO_BOOKS_ERROR);
    }

}

void print_json_book(char *book_string) {

    int books_counter = 0;

    cJSON *books = cJSON_Parse(book_string);
    cJSON *book  = NULL;

    cJSON_ArrayForEach(book, books) {

        cJSON *title       = cJSON_GetObjectItemCaseSensitive(book, TITLE);
        cJSON *author      = cJSON_GetObjectItemCaseSensitive(book, AUTHOR);
        cJSON *genre       = cJSON_GetObjectItemCaseSensitive(book, GENRE);
        cJSON *page_count  = cJSON_GetObjectItemCaseSensitive(book, PAGE_COUNT);
        cJSON *publisher   = cJSON_GetObjectItemCaseSensitive(book, PUBLISHER);

        char *title_string      = cJSON_Print(title);
        char *author_string     = cJSON_Print(author);
        char *genre_string      = cJSON_Print(genre);
        char *page_count_string = cJSON_Print(page_count);
        char *publisher_string  = cJSON_Print(publisher);

        printf("\n%s: %s\n"
               "%s: %s\n"
               "%s: %s\n"
               "%s: %s\n"
               "%s: %s\n",
               TITLE,      title_string,
               AUTHOR,     author_string,
               GENRE,      genre_string,
               PAGE_COUNT, page_count_string,
               PUBLISHER,  publisher_string);

        FREE(title_string);
        FREE(author_string);
        FREE(genre_string);
        FREE(page_count_string);
        FREE(publisher_string);

        books_counter++;

    }

    cJSON_Delete(books);

    if (books_counter == 0) {
        puts(NO_BOOKS_ERROR);
    }

}


