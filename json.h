#ifndef M2X_JSON_H
#define M2X_JSON_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct m2x_json_result {
  int status_code;
  void *data;
} m2x_json_result;

typedef enum m2x_json_status {
  M2X_JSON_OK = 0,
  M2X_JSON_INVALID = -1,
  M2X_JSON_ID_NOT_MATCH = -2,
} m2x_json_status;

typedef m2x_json_status (*m2x_json_parser) (const char *, int,
                                            const char *, int,
                                            m2x_json_result *);
typedef void* (*m2x_json_generic_parser) (const char *, int);
typedef void (*m2x_json_releaser) (void *);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_JSON_H */
