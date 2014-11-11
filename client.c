#include <string.h>

#include "m2x.h"
#include "client.h"
#include "utility.h"

/* TODO: Move these into context structs once we heard from paho-dev */
m2x_response g_message_response;
m2x_context *g_message_ctx;
char g_message_id[M2X_MESSAGE_ID_LEN + 1];
int g_got_message = 0;

/* TODO: When we have context structs, we can move this function to
 * m2x.c
 */
void message_arrived_callback(MessageData* md)
{
	MQTTMessage* message = md->message;
  m2x_json_result result;
  int s = g_message_ctx->json_parser(message->payload, message->payloadlen,
                                     g_message_id, M2X_MESSAGE_ID_LEN, &result);
  if (s == M2X_JSON_OK) {
    g_message_response.status = result.status_code;
    g_message_response.data = result.data;
    g_message_response.raw = message->payload;
    g_message_response.raw_length = message->payloadlen;
    g_got_message = 1;
  }
}

int publish_mqtt_message(m2x_context *ctx, const char *id, const char *method,
                         const char *path, const char *query, const char *body)
{
  int rc, s;
  MQTTMessage message;
  Client *client = &(ctx->client);

  if (!m2x_mqtt_is_connected(ctx)) {
    rc = m2x_mqtt_connect(ctx);
    if (rc != 0) {
      m2x_mqtt_disconnect(ctx);
      return rc;
    }
  }

  s = fill_request_buffer(ctx->assemble_buffer, g_message_id, method,
                          path, query, body);
  message.qos = 0;
  message.retained = 0;
  message.dup = 0;
  message.id = 0;
  message.payload = ctx->assemble_buffer;
  message.payloadlen = s;

  rc = MQTTPublish(client, ctx->pub_channel, &message);
  if (rc != 0) {
    m2x_mqtt_disconnect(ctx);
  }
  return rc;
}

m2x_response m2x_client_get(m2x_context *ctx, const char *path, const char *query)
{
  int rc;

  memset(&g_message_response, 0, sizeof(g_message_response));
  fill_random_hex_string(g_message_id, M2X_MESSAGE_ID_LEN);
  g_message_id[M2X_MESSAGE_ID_LEN] = '\0';
  g_message_ctx = ctx;
  g_got_message = 0;

  rc = publish_mqtt_message(ctx, g_message_id, "GET", path, query, NULL);
  if (rc != 0) {
    g_message_response.status = rc;
    return g_message_response;
  }
  while (!g_got_message) {
    rc = m2x_mqtt_yield(ctx);
    if (rc != 0) {
      g_message_response.status = rc;
      return g_message_response;
    }
  }
  if (!ctx->keepalive) {
    m2x_mqtt_disconnect(ctx);
  }
  return g_message_response;
}

m2x_response m2x_client_post(m2x_context *ctx, const char *path, const char *body)
{
  int rc;

  memset(&g_message_response, 0, sizeof(g_message_response));
  fill_random_hex_string(g_message_id, M2X_MESSAGE_ID_LEN);
  g_message_id[M2X_MESSAGE_ID_LEN] = '\0';
  g_message_ctx = ctx;
  g_got_message = 0;

  rc = publish_mqtt_message(ctx, g_message_id, "POST", path, NULL, body);
  if (rc != 0) {
    g_message_response.status = rc;
    return g_message_response;
  }
  while (!g_got_message) {
    rc = m2x_mqtt_yield(ctx);
    if (rc != 0) {
      g_message_response.status = rc;
      return g_message_response;
    }
  }
  if (!ctx->keepalive) {
    m2x_mqtt_disconnect(ctx);
  }
  return g_message_response;
}

m2x_response m2x_client_put(m2x_context *ctx, const char *path, const char *body)
{
  int rc;

  memset(&g_message_response, 0, sizeof(g_message_response));
  fill_random_hex_string(g_message_id, M2X_MESSAGE_ID_LEN);
  g_message_id[M2X_MESSAGE_ID_LEN] = '\0';
  g_message_ctx = ctx;
  g_got_message = 0;

  rc = publish_mqtt_message(ctx, g_message_id, "PUT", path, NULL, body);
  if (rc != 0) {
    g_message_response.status = rc;
    return g_message_response;
  }
  while (!g_got_message) {
    rc = m2x_mqtt_yield(ctx);
    if (rc != 0) {
      g_message_response.status = rc;
      return g_message_response;
    }
  }
  if (!ctx->keepalive) {
    m2x_mqtt_disconnect(ctx);
  }
  return g_message_response;
}

m2x_response m2x_client_delete(m2x_context *ctx, const char *path)
{
  int rc;

  memset(&g_message_response, 0, sizeof(g_message_response));
  fill_random_hex_string(g_message_id, M2X_MESSAGE_ID_LEN);
  g_message_id[M2X_MESSAGE_ID_LEN] = '\0';
  g_message_ctx = ctx;
  g_got_message = 0;

  rc = publish_mqtt_message(ctx, g_message_id, "DELETE", path, NULL, NULL);
  if (rc != 0) {
    g_message_response.status = rc;
    return g_message_response;
  }
  while (!g_got_message) {
    rc = m2x_mqtt_yield(ctx);
    if (rc != 0) {
      g_message_response.status = rc;
      return g_message_response;
    }
  }
  if (!ctx->keepalive) {
    m2x_mqtt_disconnect(ctx);
  }
  return g_message_response;
}
