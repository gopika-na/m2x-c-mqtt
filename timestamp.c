#include "client.h"
#include "timestamp.h"
#include "utility.h"

m2x_response m2x_timestamp_json(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/time", NULL);
}

m2x_response m2x_timestamp_seconds(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/time/seconds", NULL);
}

m2x_response m2x_timestamp_millis(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/time/millis", NULL);
}

m2x_response m2x_timestamp_iso8601(m2x_context *ctx)
{
  return m2x_client_get_simple(ctx, "/time/iso8601", NULL);
}
