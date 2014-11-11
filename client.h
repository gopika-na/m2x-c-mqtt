#ifndef M2X_CLIENT_H
#define M2X_CLIENT_H

#include "m2x.h"
#include "response.h"
#include "utility.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_response m2x_client_get_simple(m2x_context *ctx, const char *path, const char* query);
m2x_response m2x_client_post_simple(m2x_context *ctx, const char *path, const char *body);
m2x_response m2x_client_put_simple(m2x_context *ctx, const char *path, const char *body);
m2x_response m2x_client_delete_simple(m2x_context *ctx, const char *path);

m2x_response m2x_client_get(m2x_context *ctx, path_filling_function f, const char* args[]);
m2x_response m2x_client_post(m2x_context *ctx, path_filling_function f, const char* args[], const char *body);
m2x_response m2x_client_put(m2x_context *ctx, path_filling_function f, const char* args[], const char *body);
m2x_response m2x_client_delete(m2x_context *ctx, path_filling_function f, const char* args[]);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_CLIENT_H */
