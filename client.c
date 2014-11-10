#include <string.h>

#include "m2x.h"
#include "client.h"
#include "utility.h"

#define M2X_HOST "localhost"
#define M2X_PORT 1883

#define MESSAGE_ID_LEN 32

/* TODO: Move these into context structs once we heard from paho-dev */
m2x_response g_message_response;
m2x_context *g_message_ctx;
char g_message_id[MESSAGE_ID_LEN + 1];
int g_got_message = 0;

/* Put the buffer here to avoid stack explosion */
char g_mqtt_buffer[MQTT_BUFFER_LENGTH];

void messageArrived(MessageData* md)
{
	MQTTMessage* message = md->message;
  m2x_json_result result;
  int s = g_message_ctx->json_parser(message->payload, message->payloadlen,
                                     g_message_id, MESSAGE_ID_LEN, &result);
  if (s == M2X_JSON_OK) {
    g_message_response.status = result.status_code;
    g_message_response.data = result.data;
    g_message_response.raw = message->payload;
    g_message_response.raw_length = message->payloadlen;
    g_got_message = 1;
  }
}

m2x_response m2x_client_get(m2x_context *ctx, const char *path, const char *query)
{
  /* TODO: When keepalive is set, use existing connection instead of
   * creating a new one.
   */
  Network *network = &(ctx->network);
  Client *client = &(ctx->client);
  int rc, s;
  MQTTMessage message;
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;

  memset(&g_message_response, 0, sizeof(g_message_response));

  NewNetwork(network);
  rc = ConnectNetwork(network, M2X_HOST, M2X_PORT);
  if (rc != 0) {
    g_message_response.status = rc;
    return g_message_response;
  }
  MQTTClient(client, network, 1000,
             ctx->buf, MQTT_BUFFER_LENGTH, ctx->readbuf, MQTT_BUFFER_LENGTH);

  data.willFlag = 0;
  data.MQTTVersion = 3;
  fill_random_hex_string(g_message_id, MESSAGE_ID_LEN);
  data.clientID.cstring = g_message_id;
  data.username.cstring = ctx->key;
  data.keepAliveInterval = 60;
  data.cleansession = 1;

  rc = MQTTConnect(client, &data);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    g_message_response.status = rc;
    return g_message_response;
  }

  rc = MQTTSubscribe(client, ctx->sub_channel, 2, messageArrived);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    g_message_response.status = rc;
    return g_message_response;
  }

  fill_random_hex_string(g_message_id, MESSAGE_ID_LEN);
  g_message_id[MESSAGE_ID_LEN] = '\0';
  s = sprintf(g_mqtt_buffer, "{\"id\": \"%s\", \"method\": \"GET\", \"resource\": ", g_message_id);
  if (strlen(query) > 0) {
    s += sprintf(g_mqtt_buffer + s, "\"%s?%s\"}", path, query);
  } else {
    s += sprintf(g_mqtt_buffer + s, "\"%s\"}", path);
  }
  message.qos = 0;
  message.retained = 0;
  message.dup = 0;
  message.id = 0;
  message.payload = g_mqtt_buffer;
  message.payloadlen = s;

  g_message_ctx = ctx;
  g_got_message = 0;
  rc = MQTTPublish(client, ctx->pub_channel, &message);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    g_message_response.status = rc;
    return g_message_response;
  }
  while (!g_got_message) {
    rc = MQTTYield(client, 1000);
    if (rc != 0) {
      MQTTDisconnect(client);
      network->disconnect(network);
      g_message_response.status = rc;
      return g_message_response;
    }
  }
  return g_message_response;
}
