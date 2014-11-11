#ifndef M2X_DEVICE_H
#define M2X_DEVICE_H

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_response m2x_device_catalog(m2x_context *ctx, const char *query);
m2x_response m2x_device_list(m2x_context *ctx, const char *query);
m2x_response m2x_device_groups(m2x_context *ctx);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_DEVICE_H */
