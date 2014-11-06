#include <stdio.h>

#include "m2x.h"
#include "client.h"

const char *M2X_KEY="";

int main()
{
  m2x_context *ctx = NULL;
  char *buf = NULL;

  ctx = m2x_open(M2X_KEY);
  if (m2x_client_get(ctx, "/status", &buf) == 0) {
    printf("%s\n", buf);
    m2x_free(buf);
  }

  m2x_close(ctx);

  return 0;
}
