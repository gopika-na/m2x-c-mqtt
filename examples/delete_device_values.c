#include <stdio.h>
#include <stdlib.h>

#include "m2x.h"
#include "device.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY = "";
const char *device_id = "";
const char *stream_name = "";
const char *from = "";
const char *end = "";

int main()
{
  m2x_context ctx;
  struct json_token *arr = NULL, *tok = NULL;
  m2x_response response;
  char buffer[8192];

  json_emit(buffer, sizeof(buffer), "{s:s, s:s}",
            "from", from,
            "end", end);
  m2x_open(M2X_KEY, &ctx);
  response = m2x_device_delete_stream_value(&ctx, device_id, stream_name, buffer);
  printf("Response Status Code: %d\n", response.status);
  m2x_release_response(&ctx, &response);
  m2x_close(&ctx);
  return 0;
}
