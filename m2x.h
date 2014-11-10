#ifndef M2X_H
#define M2X_H

#include "MQTTClient.h"
#include "json.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* NOTE: Decrease this number if you are working with limited memory,
 * however, keep in mind that certain requests may not fit within the
 * smaller buffer.
 */
#define MQTT_BUFFER_LENGTH 8192

typedef struct m2x_context {
  char *pub_channel;
  char *sub_channel;
  char *key;
  int verbose;
  m2x_json_parser json_parser;

  int keepalive;

  Network network;
  Client client;
  unsigned char buf[MQTT_BUFFER_LENGTH];
  unsigned char readbuf[MQTT_BUFFER_LENGTH];
} m2x_context;

m2x_context *m2x_open(const char *key);
void m2x_close(m2x_context *ctx);

void *m2x_malloc(m2x_context *ctx, size_t len);
void *m2x_realloc(m2x_context *ctx, void *p, size_t len);
void m2x_free(void *p);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_H */
