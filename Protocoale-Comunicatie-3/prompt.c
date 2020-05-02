#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>

#include <prompt.h>
#include <memory.h>
#include <error.h>

char *read_item(char *item_name, int item_max_len, int echo) {
    /*
     * Print the prompt.
     *
     */
    printf("%s: ", item_name);
    fflush(stdout);

    struct termios current, modified;

    if (echo == ECHO_OFF) {
        /*
         * If echoing is off then disable it usint tcgetattr, tcsetattr.
         *
         */

        // get current attributes
        int tcgetattr_ret = tcgetattr(STDIN_FILENO, &current);
        NONVOID_ERROR_HANDLER(tcgetattr_ret == -1, "[ERROR] tcgetattr", NULL);

        modified = current;
        modified.c_lflag &= ~ECHO;

        int tcsetattr_ret = tcsetattr(STDIN_FILENO, TCSAFLUSH, &modified);
        NONVOID_ERROR_HANDLER(tcsetattr_ret == -1, "[ERROR] tcsetattr", NULL);
    }

    /*
     * Read at most @item_max_len chars in a buffer allocated
     * on stack.
     *
     */
    char stack_item[item_max_len];
    memset(stack_item, 0x00, item_max_len);

    fgets(stack_item, item_max_len - 1, stdin);
    stack_item[strlen(stack_item) - 1] = 0x00;

    if (echo == ECHO_OFF) {
        /*
         * Restore original settings for echoing.
         *
         */
        int tcsetattr_ret = tcsetattr(STDIN_FILENO, TCSANOW, &current);
        NONVOID_ERROR_HANDLER(tcsetattr_ret == -1, "get_auth_info: tcsetattr", NULL);
    }

    /*
     * If echoing is off then put a newline after the read is done. It makes it look
     * prettier.
     *
     */
    if (echo == ECHO_OFF) {
        puts("");
    }

    /*
     * Return a pointer on heap that will contain excactly
     * strlen + 1 bytes from @stack_item.
     *
     */
    char *heap_item;
    CALLOC(heap_item, strlen(stack_item) + 1);

    strcpy(heap_item, stack_item);
    return heap_item;

}

