#include "client.h"
#include "device.h"

m2x_response m2x_device_list(m2x_context *ctx, const char *query) {
  return m2x_client_get(ctx, "/devices", query);
}
