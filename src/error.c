#include "dac/error.h"

#include <stdarg.h>
#include <stdlib.h>

#include "dac/string.h"

void Error_Fatal(String fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[ERROR] ");
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
  exit(1);
}