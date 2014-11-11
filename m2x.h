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
#define M2X_BUFFER_LENGTH 8192
#define M2X_TIMEOUT_MS 1000
#define M2X_MESSAGE_ID_LEN 32
#define M2X_HOST "localhost"
#define M2X_PORT 1883
#define M2X_KEY_MAX_LENGTH 32
#define M2X_PUBLISH_CHANNEL_LENGTH (M2X_KEY_MAX_LENGTH + 13)
#define M2X_SUBSCRIBE_CHANNEL_LENGTH (M2X_KEY_MAX_LENGTH + 14)

typedef struct m2x_context {
  /* Buffers used to communicate with paho API, notice all buffers here
   * are NULL-terminated
   */
  char pub_channel[M2X_PUBLISH_CHANNEL_LENGTH + 1];
  char sub_channel[M2X_SUBSCRIBE_CHANNEL_LENGTH + 1];
  char key[M2X_KEY_MAX_LENGTH + 1];
  char assemble_buffer[M2X_BUFFER_LENGTH + 1];

  /* Options */
  int verbose;
  m2x_json_parser json_parser;
  int keepalive;

  /* Paho API part */
  Network network;
  Client client;
  unsigned char buf[M2X_BUFFER_LENGTH];
  unsigned char readbuf[M2X_BUFFER_LENGTH];
} m2x_context;

void m2x_open(const char *key, m2x_context *ctx);
void m2x_close(m2x_context *ctx);

int m2x_mqtt_is_connected(m2x_context *ctx);
int m2x_mqtt_connect(m2x_context *ctx);
void m2x_mqtt_disconnect(m2x_context *ctx);
int m2x_mqtt_yield(m2x_context *ctx);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_H */
