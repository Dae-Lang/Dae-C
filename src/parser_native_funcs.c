#include "dae/parser_native_funcs.h"

#include <stdio.h>

#include "dae/node.h"
#include "dae/string.h"
#include "dae/vector.h"

void Parser_Native_DeleteArgs(Vector* args) {
  Vector_Delete(args);
}

Node* Parser_Native_Print(Vector* args) {
  // well, here, the strigs a'rent okay, its cutted
  printf("Str: %s\n", (String) Vector_Get(args, 0));
  printf("SizeofStr: %lu", args->size);
  printf("\n");
  return NULL;
  for (size_t i = 0; i < args->size; i++) {
    String val = (String) Vector_Get(args, i);
    printf("%s", val);
    // if (i < args->size - 1) printf(" ");
  }
  Parser_Native_DeleteArgs(args);
  printf("\n");
  return NULL;
}
