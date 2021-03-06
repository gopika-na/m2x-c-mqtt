#include <stdlib.h>
#include <string.h>

#include "json_frozen.h"
#include "vendor/frozen/frozen.h"

m2x_json_status m2x_parse_with_frozen(const char *str, int length,
                                      const char *id_str, int id_length,
                                      m2x_json_result *result) {
  struct json_token *arr, *tok;
  int status, i, minus;

  arr = parse_json2(str, length);
  if (arr == NULL) {
    return M2X_JSON_INVALID;
  }
  tok = find_json_token(arr, "id");
  if (tok == NULL || strncmp(id_str, tok->ptr, id_length) != 0) {
    free(arr);
    return M2X_JSON_ID_NOT_MATCH;
  }
  tok = find_json_token(arr, "status");
  if (tok == NULL) {
    free(arr);
    return M2X_JSON_INVALID;
  }
  /* NOTE: We could've used atoi for this, but since status code
   * is quite easy to get, and that tok->ptr is not NULL-terminated
   * necessarily, we just hand-parse the integer to avoid allocating
   * more memory
   */
  i = 0;
  minus = 1;
  status = 0;
  if (i < tok->len && tok->ptr[i] == '-') {
    minus = -1;
    i++;
  }
  while (i < tok->len) {
    status = status * 10 + (tok->ptr[i++] - '0');
  }
  status *= minus;
  if (result == NULL) {
    free(arr);
  } else {
    result->status_code = status;
    result->data = arr;
  }
  return M2X_JSON_OK;
}

m2x_json_status m2x_parse_command_with_frozen(const char *str, int length,
                                              m2x_json_command *command) {
  struct json_token *arr, *tok;

  arr = parse_json2(str, length);
  if (arr == NULL) {
    return M2X_JSON_INVALID;
  }

  tok = find_json_token(arr, "id");
  if (tok == NULL) {
    free(arr);
    return M2X_JSON_INVALID;
  }
  command->id_ptr = tok->ptr;
  command->id_length = tok->len;

  tok = find_json_token(arr, "sent_at");
  if (tok == NULL) {
    free(arr);
    return M2X_JSON_INVALID;
  }
  command->sent_at_ptr = tok->ptr;
  command->sent_at_length = tok->len;

  command->json = (void *) arr;

  return M2X_JSON_OK;
}
