#include "client.h"
#include "distribution.h"
#include "utility.h"

m2x_response m2x_distribution_list(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/distributions", NULL);
}

m2x_response m2x_distribution_create(m2x_context *ctx, const char *data)
{
  return m2x_client_post_simple(ctx, "/distributions", data);
}

m2x_response m2x_distribution_view(m2x_context *ctx, const char *id)
{
  const char *args[3] = {"/distributions/", id, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_distribution_update(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[3] = {"/distributions/", id, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_distribution_devices(m2x_context *ctx, const char *id)
{
  const char *args[4] = {"/distributions/", id, "/devices", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_distribution_add_device(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[4] = {"/distributions/", id, "/devices", NULL};
  return m2x_client_post(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_distribution_delete(m2x_context *ctx, const char *id)
{
  const char *args[3] = {"/distributions/", id, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}

m2x_response m2x_distribution_streams(m2x_context *ctx, const char *id)
{
  const char *args[4] = {"/distributions/", id, "/streams", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_distribution_update_stream(m2x_context *ctx, const char *id, const char *name, const char *data)
{
  const char *args[5] = {"/distributions/", id, "/streams", name, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_distribution_view_stream(m2x_context *ctx, const char *id, const char *name)
{
  const char *args[5] = {"/distributions/", id, "/streams", name, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_distribution_delete_stream(m2x_context *ctx, const char *id, const char *name)
{
  const char *args[5] = {"/distributions/", id, "/streams", name, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}

m2x_response m2x_distribution_triggers(m2x_context *ctx, const char *id)
{
  const char *args[4] = {"/distributions/", id, "/triggers", NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_distribution_create_trigger(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[4] = {"/distributions/", id, "/triggers", NULL};
  return m2x_client_post(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_distribution_view_trigger(m2x_context *ctx, const char *id, const char *trigger_id)
{
  const char *args[5] = {"/distributions/", id, "/triggers/", trigger_id, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_distribution_update_trigger(m2x_context *ctx, const char *id, const char *trigger_id, const char *data)
{
  const char *args[5] = {"/distributions/", id, "/triggers/", trigger_id, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_distribution_test_trigger(m2x_context *ctx, const char *id, const char *trigger_id)
{
  const char *args[6] = {"/distributions/", id, "/triggers/", trigger_id, "/test", NULL};
  return m2x_client_post(ctx, interleave_path_and_param_fill, args, NULL);
}

m2x_response m2x_distribution_delete_trigger(m2x_context *ctx, const char *id, const char *trigger_id)
{
  const char *args[5] = {"/distributions/", id, "/triggers/", trigger_id, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}
