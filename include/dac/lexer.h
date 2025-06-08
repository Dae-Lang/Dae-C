#ifndef LEXER_H
#define LEXER_H

#include "dac/inline.h"
#include "dac/string.h"
#include "dac/vector.h"

typedef enum {
  TOKEN_KEYWORD,     // work, return
  TOKEN_IDENTIFIER,  // hello, main
  TOKEN_STRING,      // "Hello world!"
  TOKEN_LPAREN,      // (
  TOKEN_RPAREN,      // )
  TOKEN_LBRACE,      // {
  TOKEN_RBRACE,      // }
  TOKEN_ARROW,       // ->
  TOKEN_COLON,       // :
  TOKEN_TYPE,        // bool
  TOKEN_BOOLEAN,     // true, false
  TOKEN_NUMBER,      // number 1 2 3..
  TOKEN_EOF          // end of file.
} TokenType;

typedef struct {
  TokenType type;
  String text;
} Token;

DAC_INLINE static Token* Token_New(TokenType type, String text) {
  Token* tk = malloc(sizeof(Token));
  tk->type = type;
  tk->text = strdup(text);
  return tk;
}

typedef struct {
  String __input__;
  Vector* tokens;
  unsigned int __pos__;
} Lexer;

Lexer* Lexer_New(String);

void Lexer_Delete(Lexer*);

void Lexer_Tokenize(Lexer*);

#endif