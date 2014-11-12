#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utility.h"

/* Right now, this library only works with API v2 */
#define M2X_API_VERSION "/v2"

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

int path_and_query_fill(char *buffer, const char *args[])
{
  const char *path = args[0];
  const char *query = args[1];
  if (query && strlen(query) > 0) {
    return sprintf(buffer, "%s?%s", path, query);
  } else {
    return sprintf(buffer, "%s", path);
  }
}

int fill_request_buffer_with_func(char *buffer, const char *id, const char *method,
                                  path_filling_function f, const char *args[],
                                  const char *body)
{
  int s = sprintf(buffer, "{\"id\":\"%s\",\"method\":\"%s\",\"resource\":\"%s", id, method, M2X_API_VERSION);
  s += f(buffer + s, args);
  s += sprintf(buffer + s, "\"");
  if (body && strlen(body) > 0) {
    s += sprintf(buffer + s, ",\"body\":%s}", body);
  } else {
    s += sprintf(buffer + s, "}");
  }
  return s;
}

static int m2x_internal_encode_string(char *dst, const char *src)
{
  static const char *HEX = "0123456789ABCDEF";
  int len, i;

  for (i = 0, len = 0; src[i] != 0; i++) {
    if (isalnum(src[i]) ||
        (src[i] == '-') || (src[i] == '_') ||
        (src[i] == '.') || (src[i] == '~')) {
      if (dst) { dst[len] = src[i]; }
      len++;
    } else {
      if (dst) {
        dst[len] = '%';
        dst[len + 1] = HEX[src[i] / 16];
        dst[len + 2] = HEX[src[i] % 16];
      }
      len += 3;
    }
  }
  if (dst) { dst[len] = '\0'; }
  return len;
}

int interleave_path_and_param_fill(char *buffer, const char *args[])
{
  int i = 0, s = 0;
  while (args[i] != NULL) {
    if (i % 2 == 0) {
      s += sprintf(buffer + s, "%s", args[i]);
    } else {
      s += m2x_internal_encode_string(buffer + s, args[i]);
    }
    i++;
  }
  return s;
}

int query_with_interleave_fill(char *buffer, const char *args[])
{
  const char *query = args[0];
  int i = 1, s = 0;
  while (args[i] != NULL) {
    if (i % 2 == 0) {
      s += m2x_internal_encode_string(buffer + s, args[i]);
    } else {
      s += sprintf(buffer + s, "%s", args[i]);
    }
    i++;
  }
  if (query && strlen(query) > 0) {
    s += sprintf(buffer + s, "?%s", query);
  }
  return s;
}
