#include <stdio.h>
#include <stdlib.h>

#include "m2x.h"
#include "device.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY = "";
const char *device_id = "";
const char *name = "";
const char *description = "";
const char *visibility = "";
const char *groups = "";

int main()
{
  m2x_context ctx;
  struct json_token *arr = NULL, *tok = NULL;
  m2x_response response;
  char buffer[8192];

  json_emit(buffer, sizeof(buffer), "{s:s,s:s,s:s,s:s}",
            "name", name,
            "description", description,
            "visibility", visibility,
            "groups", groups);
  m2x_open(M2X_KEY, &ctx);
  response = m2x_device_update(&ctx, device_id, buffer);
  printf("Response Status Code: %d\n", response.status);
  m2x_release_response(&ctx, &response);
  m2x_close(&ctx);
  return 0;
}
