#include <string.h>

#include "m2x.h"
#include "client.h"
#include "utility.h"

#define M2X_HOST "localhost"
#define M2X_PORT 1883

#define MESSAGE_ID_LEN 32

/* TODO: Move this into context structs once we heard from paho-dev */
m2x_json_result g_message_result;
m2x_context *g_message_ctx;

char g_message_id[MESSAGE_ID_LEN + 1];
char g_mqtt_buffer[4096];

void messageArrived(MessageData* md)
{
	MQTTMessage* message = md->message;

  g_message_ctx->json_parser(message->payload, message->payloadlen,
                           g_message_id, MESSAGE_ID_LEN, &g_message_result);
}

int m2x_client_get(m2x_context *ctx, const char *path, void **out)
{
  /* TODO: When keepalive is set, use existing connection instead of
   * creating a new one.
   */
  Network *network = &(ctx->network);
  Client *client = &(ctx->client);

  NewNetwork(network);
  int rc = ConnectNetwork(network, M2X_HOST, M2X_PORT);
  if (rc != 0) {
    return rc;
  }
  MQTTClient(client, network, 1000,
             ctx->buf, MQTT_BUFFER_LENGTH, ctx->readbuf, MQTT_BUFFER_LENGTH);

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
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
    return rc;
  }

  rc = MQTTSubscribe(client, ctx->sub_channel, 2, messageArrived);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    return rc;
  }

  fill_random_hex_string(g_message_id, MESSAGE_ID_LEN);
  g_message_id[MESSAGE_ID_LEN] = '\0';
  int s = sprintf(g_mqtt_buffer, "{\"id\": \"%s\", \"method\": \"GET\", \"resource\": \"%s\"}", g_message_id, path);
  MQTTMessage message;
  message.qos = 0;
  message.retained = 0;
  message.dup = 0;
  message.id = 0;
  message.payload = g_mqtt_buffer;
  message.payloadlen = s;

  memset(&g_message_result, 0, sizeof(g_message_result));
  g_message_ctx = ctx;
  rc = MQTTPublish(client, ctx->pub_channel, &message);
  if (rc != 0) {
    MQTTDisconnect(client);
    network->disconnect(network);
    return rc;
  }
  while (g_message_result.data == NULL) {
    rc = MQTTYield(client, 1000);
    if (rc != 0) {
      MQTTDisconnect(client);
      network->disconnect(network);
      return rc;
    }
  }
  if (out != NULL) {
    *out = g_message_result.data;
  }
  return g_message_result.status_code;
}
