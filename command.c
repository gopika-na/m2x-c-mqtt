#include "m2x.h"
#include "command.h"
#include "cmdq.h"


/* TODO: move to inside the m2x_context when we have paho context structs */
cmdq_t g_cmdq;

/* Return a pointer to a new m2x_command struct, ready to fill with data.
 * The memory is not dynamically allocated, but pulled from a fixed buffer.
 * If there is no more room in the fixed buffer, NULL will be returned.
 * The m2x_command will not yet be inserted in the commands queue.
 */
m2x_command *m2x_alloc_command(m2x_context *ctx, const char *raw,
                                                 int raw_length)
{
  m2x_command *command = (m2x_command *) cmdq_node_alloc(&g_cmdq);

  if (command) {
    memcpy(command->raw, raw, raw_length);
    command->raw_length = raw_length;
  }

  return command;
}

/* Free the m2x_command struct to be available for later allocation.
 * The m2x_command should already have been popped from the commands queue.
 */
void m2x_release_command(m2x_context *ctx, m2x_command *command)
{
  if (command->json) {
    ctx->json_releaser(command->json);
    command->json = NULL;
  }

  cmdq_node_free(&g_cmdq, (cmdq_node_t *) command);
}

/* Insert the m2x_command struct into its sorted place in the commands queue.
 * The m2x_command should already have been filled with data by the json parser.
 * Commands are inserted in order by sent_at time, with more recent at the tail.
 * A command with an id that matches one in the list will not be inserted.
 */
void m2x_insert_command(m2x_context *ctx, m2x_command *command)
{
  int comparison;
  cmdq_node_t *node = (cmdq_node_t *) command;
  cmdq_node_t *other = g_cmdq.tail; /* start at the tail (most recent) */

  /* Iterate until we find a command that is older than the new one,
   * while also making sure the new command is unique (by sent_at time and id).
   */
  for (; other; other = other->prev) {
    /* If the sent_at time is older, we can insert here (break).
     * If the sent_at time is identical, we must compare the id.
     * Otherwise, (sent_at time is more recent) we continue iterating as normal.
     */
    comparison = strncmp(node->command.sent_at_ptr, other->command.sent_at_ptr,
                         node->command.sent_at_length);
    if (comparison > 0) {
      break;
    } else if (comparison == 0) {
      /* If the id is identical, just ignore it (return); we already have it. */
      comparison = strncmp(node->command.id_ptr, other->command.id_ptr,
                           node->command.id_length);
      if (comparison == 0)
        return;
    }
  }

  /* Insert node after other; if other is NULL, node will become the head. */
  cmdq_ins_after(&g_cmdq, node, other);
  return;
}
