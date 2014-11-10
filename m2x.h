#ifndef M2X_H
#define M2X_H

#include "MQTTClient.h"
#include "json.h"

#if defined(__cplusplus)
extern "C" {
#endif

#define MQTT_BUFFER_LENGTH 4096

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

void m2x_set_verbose(m2x_context *ctx, int verbose);

void *m2x_malloc(m2x_context *ctx, size_t len);
void *m2x_realloc(m2x_context *ctx, void *p, size_t len);
void m2x_free(void *p);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_H */
