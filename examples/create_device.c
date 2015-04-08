#include <stdio.h>
#include <stdlib.h>

#include "m2x.h"
#include "device.h"
#include "vendor/frozen/frozen.h"

const char *M2X_KEY="";
const char *name = "";
const char *description = "";
const char *visibility = "";
const char *tags = "";

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
            "tags", tags);
  m2x_open(M2X_KEY, &ctx);
  response = m2x_device_create(&ctx, buffer);
  printf("Response Status Code: %d\n", response.status);
  if (m2x_is_success(&response)) {
    arr = (struct json_token *) response.data;
    print_token_as_string("Device ID: ", find_json_token(arr, "body.id"));
    print_token_as_string("Device Name: ", find_json_token(arr, "body.name"));
    print_token_as_string("Device Description: ",
                          find_json_token(arr, "body.description"));
    print_token_as_string("Device Visibility: ", find_json_token(arr, "body.visibility"));
    print_token_as_string("Device Tags: ", find_json_token(arr, "body.tags"));
  }
  m2x_release_response(&ctx, &response);
  m2x_close(&ctx);
  return 0;
}
