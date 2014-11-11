#ifndef M2X_UTILITY_H
#define M2X_UTILITY_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef int (*path_filling_function)(char *buffer, const char *args[]);

int interleave_path_and_param_fill(char *buffer, const char *args[]);
int path_and_query_fill(char *buffer, const char *args[]);

void fill_random_hex_string(char *buf, int len);
int fill_request_buffer_with_func(char *buffer, const char *id, const char *method,
                                  path_filling_function f, const char *args[],
                                  const char *body);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_UTILITY_H */
