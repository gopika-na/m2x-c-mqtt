#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "m2x.h"
#include "device.h"
#include "command.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY="";
const char *device_id = "";

void print_token_as_string(const char *prefix, struct json_token *tok)
{
  if (tok == NULL) { return; }
  if (prefix) {
    printf("%s", prefix);
  }
  fwrite(tok->ptr, sizeof(char), tok->len, stdout);
  if (prefix) {
    printf("\n");
  }
}

void process_command(m2x_context *ctx, m2x_command *command)
{
  m2x_response response;
  struct json_token *arr = (struct json_token *) command->json;
  struct json_token *tok = NULL;
  char command_id[M2X_ID_MAX_LENGTH + 1];

  print_token_as_string("Processing Command: ", arr);

  /* Get the command id string. */
  command_id[0] = '\0';
  strncat(command_id, command->id_ptr, command->id_length);

  /* Verify and carry out the command here... */

  /* Mark the command as processed. */
  response = m2x_device_process_command(ctx, device_id, command_id, NULL);
  print_token_as_string("M2X API Response: ", (struct json_token *) response.data);

  /* Clean up. */
  m2x_release_response(ctx, &response);
}

#define M2X__STR_MACRO_INNER(s) #s
#define M2X__STR_MACRO(s) M2X__STR_MACRO_INNER(s)
#define M2X__FETCH_COMMANDS_PARAMS \
  "status=pending&limit=" M2X__STR_MACRO(M2X_COMMAND_QUEUE_LENGTH) ""

int m2x_fetch_pending_commands(m2x_context *ctx, const char *dev_id)
{
  m2x_response response;
  m2x_command *command;
  struct json_token *arr = NULL, *tok = NULL;
  char command_ids[M2X_COMMAND_QUEUE_LENGTH + 1][M2X_ID_MAX_LENGTH + 1];
  char token_path[30];
  int idx = 0;
  int inserted_any = 0;

  /* Fetch the summaries of the next outstanding commands, if any. */
  response = m2x_device_commands(ctx, dev_id, M2X__FETCH_COMMANDS_PARAMS);
  if (!m2x_is_success(&response))
    return 0;
  arr = (struct json_token *) response.data;

  /* Capture the command id string from each command summary in the list. */
  for (idx = 0; idx < M2X_COMMAND_QUEUE_LENGTH; idx++) {
    /* Get the id token at the token path. */
    sprintf(token_path, "body.commands[%i].id", idx);
    tok = find_json_token(arr, token_path);
    if (!tok)
      break;

    /* Store the command id string from the token. */
    command_ids[idx][0] = '\0';
    strncat(command_ids[idx], tok->ptr, tok->len);
  }
  /* Add a terminator as the last element of the command_ids array. */
  command_ids[idx][0] = '\0';

  /* Clean up. */
  m2x_release_response(ctx, &response);

  /* Use the command ids to fetch and store each detailed representation. */
  for (idx = 0; idx < M2X_COMMAND_QUEUE_LENGTH; idx++) {
    /* Break early if we've reache the terminator. */
    if (command_ids[idx][0] == '\0')
      break;

    /* Fetch the detailed representation of the command by id. */
    response = m2x_device_view_command(ctx, dev_id, command_ids[idx]);
    if (!m2x_is_success(&response))
      break;

    /* Get the representation token from the response json. */
    arr = (struct json_token *) response.data;
    tok = find_json_token(arr, "body");
    if (!tok)
      break;

    /* Create and insert the command into the commands queue. */
    command = m2x_create_command(ctx, tok->ptr, tok->len);
    if (command) {
      m2x_insert_command(ctx, command);
      inserted_any = 1;
    }

    /* Clean up. */
    m2x_release_response(ctx, &response);
    break;
  }

  /* Clean up. */
  m2x_release_response(ctx, &response);

  return inserted_any;
}

int main()
{
  m2x_context ctx;
  struct timespec last_check_time, current_time;

  m2x_open(M2X_KEY, &ctx);

  /* Set up this context for accepting incoming commands */
  ctx.keepalive = 1;
  m2x_mqtt_connect(&ctx);

  while (1) {
    m2x_command *command = m2x_next_command(&ctx);

    /* If no command is available, try fetching from remotely. */
    if (!command)
      if (m2x_fetch_pending_commands(&ctx, device_id))
        command = m2x_next_command(&ctx);

    /* If a command was found, process it. */
    if (command) {
      process_command(&ctx, command);
      m2x_release_command(&ctx, command);
    }
    else
      sleep(1); /* throttle loop speed */
  }

  m2x_close(&ctx);
  return 0;
}
