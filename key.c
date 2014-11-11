#include "client.h"
#include "key.h"
#include "utility.h"

m2x_response m2x_key_list(m2x_context *ctx, const char *query)
{
  return m2x_client_get_simple(ctx, "/keys", query);
}

m2x_response m2x_key_create(m2x_context *ctx, const char *data)
{
  return m2x_client_post_simple(ctx, "/keys", data);
}

m2x_response m2x_key_view(m2x_context *ctx, const char *key)
{
  const char *args[3] = {"/keys/", key, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_key_update(m2x_context *ctx, const char *key, const char *data)
{
  const char *args[3] = {"/keys/", key, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_key_regenerate(m2x_context *ctx, const char *key)
{
  const char *args[4] = {"/keys/", key, "/regenerate", NULL};
  return m2x_client_post(ctx, interleave_path_and_param_fill, args, NULL);
}

m2x_response m2x_key_delete(m2x_context *ctx, const char *key)
{
  const char *args[3] = {"/keys/", key, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}
