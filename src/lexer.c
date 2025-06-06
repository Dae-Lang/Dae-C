#include "dae/lexer.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dae/error.h"
#include "dae/string.h"

Lexer* Lexer_New(String input) {
  Lexer* lexer = malloc(sizeof(Lexer));
  lexer->__pos__ = 0;
  lexer->__input__ = input;
  return lexer;
}

void Lexer_Delete(Lexer* lexer) {
  Vector_Delete(lexer->tokens);
  free(lexer);
}

Token* Token_New(TokenType type, String text) {
  Token* tk = malloc(sizeof(Token));
  tk->type = type;
  tk->text = strdup(text);
  return tk;
}

void Lexer_Tokenize(Lexer* lexer) {
  size_t inputLen = String_Length(lexer->__input__);
  lexer->tokens = Vector_New(sizeof(Token));
  while (lexer->__pos__ < inputLen) {
    char c = lexer->__input__[lexer->__pos__];
    if (isspace(c)) {
      lexer->__pos__++;
      continue;
    }
    switch (c) {
      case '(':
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_LPAREN, "(")));
        lexer->__pos__++;
        continue;
      case ')':
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_RPAREN, ")")));
        lexer->__pos__++;
        continue;
      case '{':
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_LBRACE, "{")));
        lexer->__pos__++;
        continue;
      case '}':
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_RBRACE, "}")));
        lexer->__pos__++;
        continue;
      case ':':
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_COLON, ":")));
        lexer->__pos__++;
        continue;
      case ',':
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_COMMA, ",")));
        lexer->__pos__++;
        continue;
    };
    if (String_StartsWith(lexer->__input__, "->", lexer->__pos__)) {
      Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_ARROW, "->")));
      lexer->__pos__ += 2;
      continue;
    }
    if (c == '"') {
      size_t end = String_IndexOf(lexer->__input__, '"', lexer->__pos__ + 1);
      if (end == SIZE_MAX) {
        printf("Unclosed string\n");
        lexer->__pos__ = inputLen;
        break;
      }
      String str = String_Substring(lexer->__input__, lexer->__pos__ + 1, end);
      if (str == NULL) {
        printf("Failed to get substr\n");
        break;
      }
      Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_STRING, str)));
      lexer->__pos__ = end + 1;
      continue;
    }
    if (isdigit(c)) {
      size_t start = lexer->__pos__;
      while (lexer->__pos__ < inputLen &&
             isdigit(lexer->__input__[lexer->__pos__])) {
        lexer->__pos__++;
      }
      String number = String_Substring(lexer->__input__, start, lexer->__pos__);
      if (number == NULL) {
        Error_Fatal("Failed to extract number");
      }
      Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_NUMBER, number)));
      continue;
    }

    if (isalpha(c)) {
      size_t start = lexer->__pos__;
      while (lexer->__pos__ < inputLen &&
             (isalpha(lexer->__input__[lexer->__pos__]) ||
              isdigit(lexer->__input__[lexer->__pos__])))
        lexer->__pos__++;
      String word = String_Substring(lexer->__input__, start, lexer->__pos__);
      if (word == NULL) {
        Error_Fatal("Failed to get word");
        break;
      }

      if (String_Equals(word, "work") || String_Equals(word, "print") ||
          String_Equals(word, "return")) {
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_KEYWORD, word)));
      } else if (String_Equals(word, "true") || String_Equals(word, "false")) {
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_BOOLEAN, word)));
      } else if (String_Equals(word, "bool") || String_Equals(word, "int") ||
                 String_Equals(word, "string")) {
        Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_TYPE, word)));
      } else if (word != NULL) {
        Vector_PushBack(lexer->tokens,
                        ((void*)Token_New(TOKEN_IDENTIFIER, word)));
      }
      continue;
    }
    printf("Unexpected character %c\n", c);
  }
  Vector_PushBack(lexer->tokens, ((void*)Token_New(TOKEN_EOF, "")));
}