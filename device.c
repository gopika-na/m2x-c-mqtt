#include "client.h"
#include "device.h"
#include "utility.h"

m2x_response m2x_device_catalog(m2x_context *ctx, const char *query)
{
  return m2x_client_get_simple(ctx, "/devices/catalog", query);
}

m2x_response m2x_device_list(m2x_context *ctx, const char *query)
{
  return m2x_client_get_simple(ctx, "/devices", query);
}

m2x_response m2x_device_groups(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/devices/groups", NULL);
}

m2x_response m2x_device_create(m2x_context *ctx, const char *data)
{
  return m2x_client_post_simple(ctx, "/devices", data);
}

m2x_response m2x_device_update(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[3] = {"/devices/", id, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}
