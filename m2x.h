#ifndef M2X_H
#define M2X_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct m2x_context {
  char *key;
  int verbose;
} m2x_context;

m2x_context *m2x_open(const char *key);
void m2x_close(m2x_context *ctx);

void m2x_set_verbose(m2x_context *ctx, int verbose);

void *m2x_malloc(m2x_context *ctx, size_t len);
void *m2x_realloc(m2x_context *ctx, void *p, size_t len);
void m2x_free(void *p);

#if defined(__cplusplus)
}  /* extern "C" { */
#endif

#endif  /* M2X_H */
