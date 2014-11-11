#ifndef M2X_UTILITY_H
#define M2X_UTILITY_H

#if defined(__cplusplus)
extern "C" {
#endif

void fill_random_hex_string(char *buf, int len);
int fill_request_buffer(char *buffer, const char *id, const char *method,
                        const char *path, const char *query, const char *body);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_UTILITY_H */
