#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utility.h"

void fill_random_hex_string(char *buf, int len)
{
  int i, v;

  srand(time(NULL));
  for (i = 0; i < len; i++) {
    v = rand() % 36;
    if (v >= 26) {
      buf[i] = (char) ((v - 26) + '0');
    } else {
      buf[i] = (char) (v + 'A');
    }
  }
  buf[len] = '\0';
}

int fill_request_buffer(char *buffer, const char *id, const char *method,
                        const char *path, const char *query, const char *body)
{
  int s = sprintf(buffer, "{\"id\":\"%s\",\"method\":\"%s\",\"resource\":", id, method);
  if (query && strlen(query) > 0) {
    s += sprintf(buffer + s, "\"%s?%s\"", path, query);
  } else {
    s += sprintf(buffer + s, "\"%s\"", path);
  }
  if (body && strlen(body) > 0) {
    s += sprintf(buffer + s, ",\"body\":%s}", body);
  } else {
    s += sprintf(buffer + s, "}");
  }
  return s;
}
