#ifndef SUBSCRIBER_UTILS_HPP
#define SUBSCRIBER_UTILS_HPP

#include "topic.hpp"

/*
 * Format of exit request.
 *
 */
#define EXIT "EXIT"

void usage(char *filename);

/*
 * Print to stdout the received topic.
 *
 */
void print_topic(Topic *topic);

#endif

