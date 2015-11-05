#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "json_frozen.h"
#include "m2x.h"
#include "utility.h"

extern void message_arrived_callback(MessageData* md);

/* Default releaser that uses free() */
static void free_releaser(void *p)
{
  if (p) { free(p); }
}

void m2x_open(const char *key, m2x_context *ctx)
{
  memset(ctx, 0, sizeof(m2x_context));

  ctx->verbose = 0;
  ctx->keepalive = 0;
  ctx->json_parser = m2x_parse_with_frozen;
  ctx->json_generic_parser = m2x_generic_parse_with_frozen;
  ctx->json_releaser = free_releaser;
  /* Use SSL by default if at all possible */
#ifdef HAS_SSL
  ctx->use_ssl = 1;
#endif  /* HAS_SSL */

  strcpy(ctx->key, key);
  sprintf(ctx->pub_channel, "m2x/%s/requests", key);
  sprintf(ctx->sub_channel, "m2x/%s/responses", key);
}

void m2x_close(m2x_context *ctx)
{
  m2x_mqtt_disconnect(ctx);
}

int m2x_mqtt_connect(m2x_context *ctx)
{
#ifdef HAS_SSL
  Network *network;
  if (ctx->use_ssl) {
    network = (Network *) (&(ctx->network.openssl));
  } else {
    network = &(ctx->network.native);
  }
#else
  Network *network = &(ctx->network.native);
#endif  /* HAS_SSL */
  Client *client = &(ctx->client);
  int rc;
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

  if (client->isconnected) {
    return 0;
  }

#ifdef HAS_SSL
  if (ctx->use_ssl) {
    NewOpenSSLNetwork((OpenSSLNetwork *) network);
    rc = ConnectOpenSSLNetwork((OpenSSLNetwork *) network, M2X_HOST, M2X_SSL_PORT);
  } else {
    NewNetwork(network);
    rc = ConnectNetwork(network, M2X_HOST, M2X_PORT);
  }
#else
  NewNetwork(network);
  rc = ConnectNetwork(network, M2X_HOST, M2X_PORT);
#endif  /* HAS_SSL */
  if (rc != 0) {
    network->disconnect(network);
    return rc;
  }
  MQTTClient(client, network, 1000,
             ctx->buf, M2X_BUFFER_LENGTH,
             ctx->readbuf, M2X_BUFFER_LENGTH);

  data.willFlag = 0;
  data.MQTTVersion = 3;
  fill_random_hex_string(ctx->assemble_buffer, M2X_MESSAGE_ID_LEN);
  data.clientID.cstring = ctx->assemble_buffer;
  data.username.cstring = ctx->key;
  data.keepAliveInterval = 60;
  data.cleansession = 1;

  rc = MQTTConnect(client, &data);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    return rc;
  }

  rc = MQTTSubscribe(client, ctx->sub_channel, 2, message_arrived_callback);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    return rc;
  }
  return rc;
}

void m2x_mqtt_disconnect(m2x_context *ctx) {
#ifdef HAS_SSL
  Network *network;
  if (ctx->use_ssl) {
    network = (Network *) (&(ctx->network.openssl));
  } else {
    network = &(ctx->network.native);
  }
#else
  Network *network = &(ctx->network.native);
#endif  /* HAS_SSL */
  Client *client = &(ctx->client);

  if (client->isconnected) {
    MQTTDisconnect(client);
    network->disconnect(network);
  }
}

int m2x_mqtt_is_connected(m2x_context *ctx) {
  return ctx->client.isconnected;
}

int m2x_mqtt_yield(m2x_context *ctx) {
  Client *client = &(ctx->client);
  int rc;
  if (!m2x_mqtt_is_connected(ctx)) {
    return 0;
  }
  rc = MQTTYield(client, M2X_TIMEOUT_MS);
  if (rc != 0) {
    m2x_mqtt_disconnect(ctx);
  }
  return rc;
}
