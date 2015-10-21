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

m2x_response m2x_device_tags(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/devices/tags", NULL);
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

m2x_response m2x_device_view(m2x_context *ctx, const char *id)
{
  const char *args[3] = {"/devices/", id, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_location(m2x_context *ctx, const char *id)
{
  const char *args[4] = {"/devices/", id, "/location", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_update_location(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[4] = {"/devices/", id, "/location", NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_device_streams(m2x_context *ctx, const char *id)
{
  const char *args[4] = {"/devices/", id, "/streams", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_update_stream(m2x_context *ctx, const char *id, const char *name, const char *data)
{
  const char *args[5] = {"/devices/", id, "/streams", name, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_device_update_stream_value(m2x_context *ctx, const char *id, const char *name, const char *data)
{
  const char *args[6] = {"/devices/", id, "/streams/", name, "/value", NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_device_view_stream(m2x_context *ctx, const char *id, const char *name)
{
  const char *args[5] = {"/devices/", id, "/streams/", name, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_list_stream_values(m2x_context *ctx, const char *id, const char *name, const char *query)
{
  const char *args[7] = {query, "/devices/", id, "/streams/", name, "/values.json", NULL};
  return m2x_client_get(ctx, query_with_interleave_fill, args);
}

m2x_response m2x_device_stream_sampling(m2x_context *ctx, const char *id, const char *name, const char *query)
{
  const char *args[7] = {query, "/devices/", id, "/streams/", name, "/sampling.json", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_stream_stats(m2x_context *ctx, const char *id, const char *name, const char *query)
{
  const char *args[7] = {query, "/devices/", id, "/streams/", name, "/stats", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_post_stream_value(m2x_context *ctx, const char *id, const char *name, const char *data)
{
  const char *args[6] = {"/devices/", id, "/streams/", name, "/values", NULL};
  return m2x_client_post(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_device_delete_stream_value(m2x_context *ctx, const char *id, const char *name, const char *data)
{
  const char *args[6] = {"/devices/", id, "/streams/", name, "/values", NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_device_delete_stream(m2x_context *ctx, const char *id, const char *name)
{
  const char *args[5] = {"/devices/", id, "/streams/", name, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}

m2x_response m2x_device_post_updates(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[4] = {"/devices/", id, "/updates", NULL};
  return m2x_client_post(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_device_view_log(m2x_context *ctx, const char *id)
{
  const char *args[4] = {"/devices/", id, "/log", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_device_delete(m2x_context *ctx, const char *id)
{
  const char *args[3] = {"/devices/", id, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}
