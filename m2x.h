/** @file m2x.h
 *  @brief Wrapper for <a href="https://m2x.att.com/developer/documentation/v2/overview">AT&T M2X API</a>
 */

/*! \mainpage C-MQTT toolkit for the <a href="https://m2x.att.com/developer/documentation/v2/overview">AT&T M2X API</a>
 *
 * View the <a href="https://github.com/attm2x/m2x-c-mqtt/blob/master/README.md">M2X C-MQTT Client README</a> for usage details
 *
 * All methods in this client library require an API Key for authentication. There are multiple types of API Keys which provide granular access to your M2X resources. Please review the <a href="https://m2x.att.com/developer/documentation/v2/overview#API-Keys">API Keys documentation</a> for more details on the different types of keys available
 *
 * If an invalid API Key is utilized, you will receive the following error when calling client methods:
 *
 * ```javascript
 * >>>client.method(...)
 * Traceback (most recent call last):
 * ...
 * requests.exceptions.HTTPError: 401 Client Error: Unauthorized
 * ```
 */
#ifndef M2X_H
#define M2X_H

#include "MQTTClient.h"
#include "json.h"

#ifdef HAS_SSL
#include "openssl.h"
#endif  /* HAS_SSL */

#if defined(__cplusplus)
extern "C" {
#endif

/* NOTE: Decrease this number if you are working with limited memory,
 * however, keep in mind that certain requests may not fit within the
 * smaller buffer.
 */
#define M2X_BUFFER_LENGTH 8192
#define M2X_COMMAND_BUFFER_LENGTH 2048
#define M2X_COMMAND_QUEUE_LENGTH 100
#define M2X_TIMEOUT_MS 100
/*
 * Since per-yeild timeout is 100 milliseconds, doing a maximum of 20
 * yields means our request-level timeout is roughly 2000 milliseconds,
 * this is not very accurate but a simple solution that works.
 */
#define M2X_REQUEST_MAX_YIELDS 20
#define M2X_MESSAGE_ID_LEN 32
#define M2X_HOST "api-m2x.att.com"
#define M2X_PORT 1883
#define M2X_KEY_MAX_LENGTH 32
#define M2X_ID_MAX_LENGTH 38
#define M2X_PUBLISH_CHANNEL_LENGTH (M2X_KEY_MAX_LENGTH + 13)
#define M2X_SUBSCRIBE_CHANNEL_LENGTH (M2X_KEY_MAX_LENGTH + 14)
#define M2X_COMMANDS_CHANNEL_LENGTH (M2X_KEY_MAX_LENGTH + 13)

/*
 * Error codes defined here shouldn't conflict with Paho's returnCode
 */
#define M2X_ERROR_TIMEOUT -3

#ifdef HAS_SSL
#define M2X_SSL_PORT 8883
#endif  /* HAS_SSL */

typedef struct m2x_context {
  /* Buffers used to communicate with paho API, notice all buffers here
   * are NULL-terminated
   */
  char pub_channel[M2X_PUBLISH_CHANNEL_LENGTH + 1];
  char sub_channel[M2X_SUBSCRIBE_CHANNEL_LENGTH + 1];
  char commands_channel[M2X_COMMANDS_CHANNEL_LENGTH + 1];
  char key[M2X_KEY_MAX_LENGTH + 1];
  char assemble_buffer[M2X_BUFFER_LENGTH + 1];

  /* Options */
  int verbose;
  int keepalive;
  m2x_json_parser json_parser;
  m2x_json_command_parser json_command_parser;
  m2x_json_releaser json_releaser;
#ifdef HAS_SSL
  int use_ssl;
#endif  /* HAS_SSL */

  /* Current status flags */
  int commands_overflow; /* nonzero when received commands have been dropped */

  /* Paho API part */
  union {
    Network native;
#ifdef HAS_SSL
    OpenSSLNetwork openssl;
#endif  /* HAS_SSL */
  } network;
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
int m2x_mqtt_yield_nonblock(m2x_context *ctx);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_H */
