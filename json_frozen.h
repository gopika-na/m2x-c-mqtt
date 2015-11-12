#ifndef M2X_JSON_FROZEN_H
#define M2X_JSON_FROZEN_H

#include "json.h"

#if defined(__cplusplus)
extern "C" {
#endif

m2x_json_status m2x_parse_with_frozen(const char *str, int length,
                                      const char *id_str, int id_length,
                                      m2x_json_result *result);

m2x_json_status m2x_parse_command_with_frozen(const char *str, int length,
                                              m2x_json_command *command);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_JSON_FROZEN_H */
