#include <stdio.h>
#include <stdlib.h>

#include "m2x.h"
#include "device.h"
#include "thirdparty/frozen/frozen.h"

const char *M2X_KEY="";

#define INTEGER_BUFFER_LEN 20

int parse_integer(struct json_token *tok) {
  static char buf[INTEGER_BUFFER_LEN + 1];
  int len = INTEGER_BUFFER_LEN, i;
  if (tok == NULL) { return 0; }
  if (len > tok->len) {
    len = tok->len;
  }
  for (i = 0; i < len; i++) {
    buf[i] = tok->ptr[i];
  }
  buf[len] = '\0';
  return atoi(buf);
}

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
  m2x_context *ctx = NULL;
  struct json_token *arr = NULL, *tok = NULL, *tok2 = NULL;
  int i, len;
  m2x_response response;
  char buf[40];

  ctx = m2x_open(M2X_KEY);
  response = m2x_device_list(ctx, "");
  printf("Response Status Code: %d\n", response.status);
  if (m2x_is_success(&response)) {
    arr = (struct json_token *) response.data;
    printf("Total: %d\n", parse_integer(find_json_token(arr, "body.total")));
    printf("Pages: %d\n", parse_integer(find_json_token(arr, "body.pages")));
    printf("Limit: %d\n\n", parse_integer(find_json_token(arr, "body.limit")));
    tok = find_json_token(arr, "body.devices");
    if (tok) {
      i = 0;
      while (1) {
        sprintf(buf, "[%d].id", i);
        tok2 = find_json_token(tok, buf);
        if (!tok2) { break; }
        print_token_as_string("Device ID: ", tok2);
        sprintf(buf, "[%d].name", i);
        tok2 = find_json_token(tok, buf);
        print_token_as_string("Device Name: ", tok2);
        printf("\n");
        i++;
      }
    }
    free(arr);
  }

  m2x_close(ctx);

  return 0;
}
