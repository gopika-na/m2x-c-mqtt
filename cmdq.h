/** @file cmdq.h
 */

#ifndef M2X_CMDQ_H
#define M2X_CMDQ_H

#include "m2x.h"
#include "command.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define CMDQ_MAX_NODES M2X_COMMAND_QUEUE_LENGTH

struct cmdq_node_t;

/* Represents a single m2x_command node within the context of a cmdq_t. */
typedef struct cmdq_node_t {
  /* The m2x_command member must always be the first member,
   * so that we can freely cast between pointers of both types.
   */
  m2x_command command;

  struct cmdq_node_t *next;
  struct cmdq_node_t *prev;
} cmdq_node_t;

/* Represents a low-level doubly-linked list containing cmdq_node_t items.
 * The maximum number of items is static (CMDQ_MAX_NODES), so there is no
 * dynamic memory allocation, just marking of nodes that are available for use.
 */
typedef struct cmdq_t {
  cmdq_node_t array[CMDQ_MAX_NODES];
  cmdq_node_t *head;
  cmdq_node_t *tail;
  size_t next_try_idx;
} cmdq_t;

void cmdq_init(cmdq_t *cmdq);
cmdq_node_t *cmdq_node_alloc(cmdq_t *cmdq);
void cmdq_node_free(cmdq_t *cmdq, cmdq_node_t *node);
void cmdq_ins_after(cmdq_t *cmdq, cmdq_node_t *node, cmdq_node_t *other);
void cmdq_ins_before(cmdq_t *cmdq, cmdq_node_t *node, cmdq_node_t *other);
void cmdq_pop(cmdq_t *cmdq, cmdq_node_t *node);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_CMDQ_H */
