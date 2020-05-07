#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"
#include <unistd.h>

//struct queue;
struct queue
{
  list head;
  list tail;
  size_t size;
};
typedef struct queue *queue;

/* create an empty queue */
extern queue queue_create(void);

/* insert an element at the end of the queue */
extern void queue_enq(queue q, void *element);

/* delete the front element on the queue and return it */
extern void *queue_deq(queue q);

/* return a true value if and only if the queue is empty */
extern int queue_empty(queue q);

extern void delete_queue(queue q);

#endif
