#include "queue.h"
//#include "list.h"
#include <stdlib.h>
#include <assert.h>


queue queue_create(void)
{
  queue q = malloc(sizeof(struct queue));
  q -> head = q -> tail = NULL;
  q -> size = 0;
  return q;
}

int queue_empty(queue q)
{
  return q -> head == NULL;
}

void queue_enq(queue q, void *element)
{
  if(queue_empty(q))
    q -> head = q->tail = cons(element, NULL);
  else
    {
      q->tail->next = cons(element, NULL);
      q->tail = q->tail->next;
    }

  q -> size ++;
}

void *queue_deq(queue q)
{
  assert(!queue_empty(q));
  {
    void *temp = q->head->element;
    q -> head = cdr_and_free(q->head);
    q -> size --;
    return temp;
  }
}

void delete_queue(queue q) {
    list head = q->head;

    while (head) {
        free(head->element);
        head = cdr_and_free(head);
    }

    free(q);
}
