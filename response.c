#include "response.h"

int m2x_is_success(const m2x_response *response)
{
  return response->status >= 200 && response->status < 300;
}

int m2x_is_client_error(const m2x_response *response)
{
  return (response->status >= 400 && response->status < 500) ||
      response->status < 0;
}

int m2x_is_server_error(const m2x_response *response)
{
  return response->status >= 500;
}

int m2x_is_error(const m2x_response *response)
{
  return m2x_is_client_error(response) || m2x_is_server_error(response);
}

void m2x_release_response(m2x_context *ctx, m2x_response *response)
{
  ctx->json_releaser(response->data);
  response->data = NULL;
}
