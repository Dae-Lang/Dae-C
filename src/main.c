#include <stdio.h>

#include "dac/file.h"
#include "dac/string.h"
#include "dac/tokenizer.h"

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Please provide at least 1 file.\n");
    return 1;
  }

  File* srcFile = File_Open(argv[1], FileMode_Read);
  String src = File_ReadText(srcFile);

  Tokenizer* tokenizer = Tokenizer_New(src);
  Tokenizer_Tokenize(tokenizer);
  for (unsigned int i = 0; i < tokenizer->tokens->size; i++) {
    Token* token = (Token*)Vector_Get(tokenizer->tokens, i);
    printf("%d: {\n  Text: %s,\n  Type: %d\n}\n", i, token->text, token->type);
  }
  Tokenizer_Delete(tokenizer);
  File_Close(srcFile);
  return 0;
}