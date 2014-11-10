#include <stdio.h>

#include "m2x.h"
#include "client.h"
#include "thirdparty/frozen/frozen.h"

const char *M2X_KEY="";

int main()
{
  m2x_context *ctx = NULL;
  struct json_token *arr = NULL, *tok = NULL;
  int i, status;

  ctx = m2x_open(M2X_KEY);
  status = m2x_client_get(ctx, "/status", (void**) &arr);
  printf("Response Status Code: %d\n", status);
  if (status > 0) {
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
    free(arr);
  }

  m2x_close(ctx);

  return 0;
}
