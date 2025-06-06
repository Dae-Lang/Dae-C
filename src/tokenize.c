#include "dac/tokenizer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dac/error.h"
#include "dac/string.h"

Tokenizer* Tokenizer_New(String input) {
  Tokenizer* tokenizer = malloc(sizeof(Tokenizer));
  tokenizer->__pos__ = 0;
  tokenizer->__input__ = input;
  return tokenizer;
}

void Tokenizer_Delete(Tokenizer* tokenizer) {
  Vector_Delete(tokenizer->tokens);
  free(tokenizer);
}

void Tokenizer_Tokenize(Tokenizer* tokenizer) {
  size_t inputLen = String_Length(tokenizer->__input__);
  tokenizer->tokens = Vector_New(sizeof(Token));
  while (tokenizer->__pos__ < inputLen) {
    char c = tokenizer->__input__[tokenizer->__pos__];
    if (isspace(c)) {
      tokenizer->__pos__++;
      continue;
    }
    switch (c) {
      case '(':
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(LPAREN, "(")));
        tokenizer->__pos__++;
        continue;
      case ')':
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(RPAREN, ")")));
        tokenizer->__pos__++;
        continue;
      case '{':
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(LBRACE, "{")));
        tokenizer->__pos__++;
        continue;
      case '}':
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(RBRACE, "}")));
        tokenizer->__pos__++;
        continue;
      case ':':
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(COLON, ":")));
        tokenizer->__pos__++;
        continue;
    };
    if (String_StartsWith(tokenizer->__input__, "->", tokenizer->__pos__)) {
      Vector_PushBack(tokenizer->tokens, ((void*)Token_New(ARROW, "->")));
      tokenizer->__pos__ += 2;
      continue;
    }
    if (c == '"') {
      unsigned int end =
          String_IndexOf(tokenizer->__input__, '"', tokenizer->__pos__ + 1);
      if (end == SIZE_MAX) {
        printf("Unclosed string\n");
        tokenizer->__pos__ = inputLen;
        break;
      }
      String str =
          String_Substring(tokenizer->__input__, tokenizer->__pos__ + 1, end);
      if (str == NULL) {
        printf("Failed to get substr\n");
        break;
      }
      Vector_PushBack(tokenizer->tokens, ((void*)Token_New(STRING, str)));
      tokenizer->__pos__ = end + 1;
      continue;
    }

    if (isalpha(c)) {
      size_t start = tokenizer->__pos__;
      while (tokenizer->__pos__ < inputLen &&
             (isalpha(tokenizer->__input__[tokenizer->__pos__]) ||
              isdigit(tokenizer->__input__[tokenizer->__pos__])))
        tokenizer->__pos__++;
      String word =
          String_Substring(tokenizer->__input__, start, tokenizer->__pos__);
      if (word == NULL) {
        Error_Fatal("Failed to get word");
        break;
      }

      if (String_Equals(word, "work") || String_Equals(word, "print") ||
          String_Equals(word, "return")) {
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(KEYWORD, word)));
      } else if (String_Equals(word, "true") || String_Equals(word, "false")) {
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(BOOLEAN, word)));
      } else if (String_Equals(word, "bool") || String_Equals(word, "int") ||
                 String_Equals(word, "string")) {
        Vector_PushBack(tokenizer->tokens, ((void*)Token_New(TYPE, word)));
      } else if (word != NULL) {
        Vector_PushBack(tokenizer->tokens,
                        ((void*)Token_New(IDENTIFIER, word)));
      }
      continue;
    }
    printf("Unexpected character %c\n", c);
  }
  Vector_PushBack(tokenizer->tokens, ((void*)Token_New(EOF, "")));
}