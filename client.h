#ifndef M2X_CLIENT_H
#define M2X_CLIENT_H

#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_response m2x_client_get(m2x_context *ctx, const char *path);
int m2x_client_post(m2x_context *ctx, const char *path, const char *data, char **out);
int m2x_client_put(m2x_context *ctx, const char *path, const char *data, char **out);
int m2x_client_delete(m2x_context *ctx, const char *path, char **out);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_CLIENT_H */
