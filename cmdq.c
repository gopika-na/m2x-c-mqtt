#include "cmdq.h"

#define CMDQ_POISON1 ((void *) 0x100)
#define CMDQ_POISON2 ((void *) 0x200)

/* Set up a cmdq_t struct. This should be called exactly once before using.
 */
void cmdq_init(cmdq_t *cmdq)
{
  size_t idx;
  for (idx = 0; idx < CMDQ_MAX_NODES; idx++) {
    cmdq->array[idx].next = CMDQ_POISON1;
    cmdq->array[idx].prev = CMDQ_POISON2;
  }

  cmdq->head = NULL;
  cmdq->tail = NULL;
  cmdq->next_try_idx = 0;
}

/* Returns an unused node from the cmdq->array, or NULL if none are free.
 * This can be treated like an allocation from static memory.
 */
cmdq_node_t *cmdq_node_alloc(cmdq_t *cmdq)
{
  cmdq_node_t *node = NULL;
  size_t try_idx = cmdq->next_try_idx;
  size_t try_count = 0;

  for (; ; try_idx++) {
    if (try_idx >= CMDQ_MAX_NODES)
      try_idx = 0; /* we've wrapped around the end; start at the beginning. */

    if (try_count >= CMDQ_MAX_NODES)
      return NULL; /* we've checked them all; there are no free nodes. */
    try_count++;

    node = cmdq->array + try_idx;
    if (CMDQ_POISON1 == node->next && CMDQ_POISON2 == node->prev)
      break; /* we've found a free node at this index. */
  }

  cmdq->next_try_idx = try_idx + 1;

  /* TODO: zero all memory in the entire struct */
  node->next = NULL;
  node->prev = NULL;
  return node;
}

/* Free an unused node reference that was created by cmdq_node_alloc.
 * Safe to call with node == NULL.
 */
void cmdq_node_free(cmdq_t *cmdq, cmdq_node_t *node)
{
  if (!node)
    return;

  node->next = CMDQ_POISON1;
  node->prev = CMDQ_POISON2;
}

/* Insert the given node into the list immediately after the other given node.
 * If other is NULL, then node becomes the head of the list.
 * If other is NULL and there is no tail, then node becomes the tail as well.
 */
void cmdq_ins_after(cmdq_t *cmdq, cmdq_node_t *node, cmdq_node_t *other)
{
  if (other) {
    other->next->prev = node;
    node->next = other->next;
    node->prev = other;
    other->next = node;
  } else {
    if (cmdq->head) {
      cmdq->head->prev = node;
      node->prev = cmdq->head;
    }

    cmdq->head = node;

    if (!cmdq->tail)
      cmdq->tail = node;
  }
}

/* Insert the given node into the list immediately before the other given node.
 * If other is NULL, then node becomes the tail of the list.
 * If other is NULL and there is no head, then node becomes the head as well.
 */
void cmdq_ins_before(cmdq_t *cmdq, cmdq_node_t *node, cmdq_node_t *other)
{
  if (other) {
    other->prev->next = node;
    node->prev = other->prev;
    node->next = other;
    other->prev = node;
  } else {
    if (cmdq->tail) {
      cmdq->tail->next = node;
      node->next = cmdq->tail;
    }

    cmdq->tail = node;

    if (!cmdq->head)
      cmdq->head = node;
  }
}

/* Remove a node from the list; the node must still be freed in a separate step.
 * Safe to call with node == NULL.
 */
void cmdq_pop(cmdq_t *cmdq, cmdq_node_t *node)
{
  if (!node)
    return;

  if (node == cmdq->head)
    cmdq->head = node->next;
  if (node == cmdq->tail)
    cmdq->tail = node->prev;

  if (node->next)
    node->next->prev = node->prev;
  if (node->prev)
    node->prev->next = node->next;

  node->next = NULL;
  node->prev = NULL;
}
