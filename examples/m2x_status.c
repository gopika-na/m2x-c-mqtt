#include <stdio.h>

#include "m2x.h"
#include "client.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY="";

int main()
{
  m2x_context ctx;
  struct json_token *arr = NULL, *tok = NULL;
  int i, status;
  m2x_response response;

  m2x_open(M2X_KEY, &ctx);
  response = m2x_client_get_simple(&ctx, "/status", NULL);
  printf("Response Status Code: %d\n", response.status);
  if (m2x_is_success(&response)) {
    arr = (struct json_token *) response.data;
    tok = find_json_token(arr, "body.api");
    if (tok != NULL) {
      printf("API Status: ");
      fwrite(tok->ptr, sizeof(char), tok->len, stdout);
      printf("\n");
    }
    tok = find_json_token(arr, "body.triggers");
    if (tok != NULL) {
      printf("Triggers Status: ");
      fwrite(tok->ptr, sizeof(char), tok->len, stdout);
      printf("\n");
    }
  }
  m2x_release_response(&ctx, &response);
  m2x_close(&ctx);
  return 0;
}
