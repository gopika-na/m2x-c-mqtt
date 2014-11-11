#include "client.h"
#include "device.h"

m2x_response m2x_device_catalog(m2x_context *ctx, const char *query)
{
  return m2x_client_get(ctx, "/devices/catalog", query);
}

m2x_response m2x_device_list(m2x_context *ctx, const char *query)
{
  return m2x_client_get(ctx, "/devices", query);
}

m2x_response m2x_device_groups(m2x_context *ctx)
{
  return m2x_client_get(ctx, "/devices/groups", NULL);
}
