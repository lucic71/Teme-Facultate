#ifndef PROMPT_H_
#define PROMPT_H_

/*
 * Prompts the user the message @item_name and waits for an input
 * of at most @item_max_len bytes.
 *
 * Returns a heap memory array that will contain the user data.
 *
 */
#define ECHO_ON  1
#define ECHO_OFF 0

char *read_item(char *item_name, int item_max_len, int echo);

#endif
