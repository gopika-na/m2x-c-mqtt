#include <stdio.h>
#include <stdlib.h>

#include "m2x.h"
#include "timestamp.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY="";

void print_token_as_string(const char *prefix, struct json_token *tok) {
  if (tok == NULL) { return; }
  if (prefix) {
    printf("%s", prefix);
  }
  fwrite(tok->ptr, sizeof(char), tok->len, stdout);
  if (prefix) {
    printf("\n");
  }
}

int main()
{
  m2x_context ctx;
  struct json_token *tok = NULL;
  int i, len;
  m2x_response response;
  char buf[40];

  m2x_open(M2X_KEY, &ctx);

  response = m2x_timestamp_seconds(&ctx);
  if (m2x_is_success(&response)) {
    tok = (struct json_token *) response.data;
    print_token_as_string("Current seconds: ", find_json_token(tok, "body"));
  }
  m2x_release_response(&ctx, &response);

  response = m2x_timestamp_millis(&ctx);
  if (m2x_is_success(&response)) {
    tok = (struct json_token *) response.data;
    print_token_as_string("Current millis: ", find_json_token(tok, "body"));
  }
  m2x_release_response(&ctx, &response);

  response = m2x_timestamp_iso8601(&ctx);
  if (m2x_is_success(&response)) {
    tok = (struct json_token *) response.data;
    print_token_as_string("Current ISO8601 timestamp: ", find_json_token(tok, "body"));
  }
  m2x_release_response(&ctx, &response);

  m2x_close(&ctx);
  return 0;
}
