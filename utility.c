#include <stdlib.h>
#include <time.h>

#include "utility.h"

void fill_random_hex_string(char *buf, int len) {
  int i, v;

  srand(time(NULL));
  for (i = 0; i < len; i++) {
    v = rand() % 36;
    if (v >= 26) {
      buf[i] = (char) ((v - 26) + '0');
    } else {
      buf[i] = (char) (v + 'A');
    }
  }
  buf[len] = '\0';
}
