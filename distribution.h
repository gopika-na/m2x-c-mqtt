#ifndef M2X_DISTRIBUTION_H
#define M2X_DISTRIBUTION_H

#include "m2x.h"
#include "response.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_response m2x_distribution_list(m2x_context *ctx);
m2x_response m2x_distribution_create(m2x_context *ctx, const char *data);
m2x_response m2x_distribution_view(m2x_context *ctx, const char *id);
m2x_response m2x_distribution_update(m2x_context *ctx, const char *id, const char *data);
m2x_response m2x_distribution_delete(m2x_context *ctx, const char *id);

m2x_response m2x_distribution_devices(m2x_context *ctx, const char *id);
m2x_response m2x_distribution_add_device(m2x_context *ctx, const char *id, const char *data);

m2x_response m2x_distribution_streams(m2x_context *ctx, const char *id);
m2x_response m2x_distribution_update_stream(m2x_context *ctx, const char *id, const char *name, const char *data);
#define m2x_distribution_create_stream m2x_distribution_update_stream
m2x_response m2x_distribution_view_stream(m2x_context *ctx, const char *id, const char *name);
m2x_response m2x_distribution_delete_stream(m2x_context *ctx, const char *id, const char *name);

m2x_response m2x_distribution_triggers(m2x_context *ctx, const char *id);
m2x_response m2x_distribution_create_trigger(m2x_context *ctx, const char *id, const char *data);
m2x_response m2x_distribution_view_trigger(m2x_context *ctx, const char *id, const char *trigger_id);
m2x_response m2x_distribution_update_trigger(m2x_context *ctx, const char *id, const char *trigger_id, const char *data);
m2x_response m2x_distribution_test_trigger(m2x_context *ctx, const char *id, const char *trigger_id);
m2x_response m2x_distribution_delete_trigger(m2x_context *ctx, const char *id, const char *trigger_id);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_DISTRIBUTION_H */

