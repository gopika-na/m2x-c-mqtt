#ifndef M2X_TIMESTAMP_H
#define M2X_TIMESTAMP_H

#include <time.h>

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_response m2x_timestamp_json(m2x_context *ctx);

m2x_response m2x_timestamp_seconds(m2x_context *ctx);
m2x_response m2x_timestamp_millis(m2x_context *ctx);
m2x_response m2x_timestamp_iso8601(m2x_context *ctx);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_TIMESTAMP_H */
