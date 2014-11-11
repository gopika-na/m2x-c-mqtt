#ifndef M2X_KEY_H
#define M2X_KEY_H

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_response m2x_key_list(m2x_context *ctx, const char *query);
m2x_response m2x_key_create(m2x_context *ctx, const char *data);
m2x_response m2x_key_view(m2x_context *ctx, const char *key);
m2x_response m2x_key_update(m2x_context *ctx, const char *key, const char *data);
m2x_response m2x_key_regenerate(m2x_context *ctx, const char *key);
m2x_response m2x_key_delete(m2x_context *ctx, const char *key);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_KEY_H */
