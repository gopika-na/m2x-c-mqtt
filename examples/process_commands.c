#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "m2x.h"
#include "device.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY="";
const char *device_id = "";

#define COMMAND_CHECK_INTERVAL 10 /* seconds */

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

void process_command(m2x_context *ctx, void *json)
{
  m2x_response response;
  struct json_token *arr = (struct json_token *) json;
  struct json_token *tok = NULL;
  char command_id[M2X_ID_MAX_LENGTH + 1];

  print_token_as_string("Processing Command: ", arr);

  /* Get the command id string. */
  tok = find_json_token(arr, "id");
  command_id[0] = '\0';
  strncat(command_id, tok->ptr, tok->len);

  /* Mark the command as processed. */
  response = m2x_device_process_command(ctx, device_id, command_id, NULL);
  print_token_as_string("M2X API Response: ", (struct json_token *) response.data);

  /* Clean up. */
  m2x_release_response(ctx, &response);
}

void check_for_unacknowledged_commands(m2x_context *ctx)
{
  m2x_response response;
  struct json_token *arr = NULL, *tok = NULL;
  char command_id[M2X_ID_MAX_LENGTH + 1];

  while (1) {
    /* Fetch the summary of the next outstanding command, if any. */
    response = m2x_device_commands(ctx, device_id, "status=sent&limit=1");
    if (!m2x_is_success(&response))
      break;

    /* Get the id token from the command summary json. */
    arr = (struct json_token *) response.data;
    tok = find_json_token(arr, "body.commands[0].id");
    if (!tok)
      break;

    /* Store the command id string from the token. */
    command_id[0] = '\0';
    strncat(command_id, tok->ptr, tok->len);

    /* Clean up. */
    m2x_release_response(ctx, &response);

    /* Fetch the detailed representation of the command by id. */
    response = m2x_device_view_command(ctx, device_id, command_id);
    if (!m2x_is_success(&response))
      break;

    /* Process the command. */
    arr = (struct json_token *) response.data;
    process_command(ctx, (void *) find_json_token(arr, "body"));

    /* Clean up. */
    m2x_release_response(ctx, &response);
  }

  /* Clean up. */
  m2x_release_response(ctx, &response);
}

int main()
{
  m2x_context ctx;
  struct timespec last_check_time, current_time;

  m2x_open(M2X_KEY, &ctx);

  /* Set up this context for accepting incoming commands */
  ctx.keepalive = 1;
  ctx.command_handler = process_command;

  /* Check for unacknowledged commands on process start. */
  check_for_unacknowledged_commands(&ctx);
  clock_gettime(CLOCK_MONOTONIC, &last_check_time);

  while (1) {
    /* Yield to MQTT to process any incoming command notifications. */
    m2x_mqtt_yield(&ctx);

    /* Check for unacknowledged commands every COMMAND_CHECK_INTERVAL seconds */
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    if (current_time.tv_sec >= last_check_time.tv_sec + COMMAND_CHECK_INTERVAL){
      check_for_unacknowledged_commands(&ctx);
      clock_gettime(CLOCK_MONOTONIC, &last_check_time);
    }

    /* Do any other work here. */
    printf("%s\n", "...");

    /* Throttle loop speed */
    sleep(1);
  }

  m2x_close(&ctx);
  return 0;
}
