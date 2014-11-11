#include "client.h"
#include "chart.h"
#include "utility.h"

m2x_response m2x_chart_list(m2x_context *ctx, const char *query)
{
  return m2x_client_get_simple(ctx, "/charts", query);
}

m2x_response m2x_chart_create(m2x_context *ctx, const char *data)
{
  return m2x_client_post_simple(ctx, "/charts", data);
}

m2x_response m2x_chart_view(m2x_context *ctx, const char *id)
{
  const char *args[3] = {"/charts/", id, NULL};
  return m2x_client_get(ctx, interleave_path_and_param_fill, args);
}

m2x_response m2x_chart_update(m2x_context *ctx, const char *id, const char *data)
{
  const char *args[3] = {"/charts/", id, NULL};
  return m2x_client_put(ctx, interleave_path_and_param_fill, args, data);
}

m2x_response m2x_chart_delete(m2x_context *ctx, const char *id)
{
  const char *args[3] = {"/charts/", id, NULL};
  return m2x_client_delete(ctx, interleave_path_and_param_fill, args, NULL);
}
