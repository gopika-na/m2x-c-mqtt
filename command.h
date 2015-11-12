#ifndef M2X_COMMAND_H
#define M2X_COMMAND_H

#include "m2x.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Represents a received command, with just enough tokenized information to
 * check uniqueness (id_ptr, id_length) and sort (sent_at_ptr, sent_at_length).
 */
typedef struct m2x_command {
  char raw[M2X_COMMAND_BUFFER_LENGTH];
  int raw_length;
  const char *id_ptr;
  int id_length;
  const char *sent_at_ptr;
  int sent_at_length;
  void *json;
} m2x_command;

m2x_command *m2x_alloc_command(m2x_context *ctx, const char *raw,
                                                 int raw_length);
void m2x_release_command(m2x_context *ctx, m2x_command *command);
void m2x_insert_command(m2x_context *ctx, m2x_command *command);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_COMMAND_H */
