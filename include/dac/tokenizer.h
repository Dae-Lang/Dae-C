#ifndef TOKENIZE_H
#define TOKENIZE_H

#include "dac/inline.h"
#include "dac/string.h"
#include "dac/vector.h"

typedef enum {
  KEYWORD,     // work, return
  IDENTIFIER,  // hello, main
  STRING,      // "Hello world!"
  LPAREN,      // (
  RPAREN,      // )
  LBRACE,      // {
  RBRACE,      // }
  ARROW,       // ->
  COLON,       // :
  TYPE,        // bool
  BOOLEAN      // true, false
} TokenType;

typedef struct {
  TokenType type;
  String text;
} Token;

DAC_INLINE static Token* Token_New(TokenType type, String text) {
  Token* tk = malloc(sizeof(Token));
  tk->type = type;
  tk->text = text;
  return tk;
}

typedef struct {
  String __input__;
  Vector* tokens;
  unsigned int __pos__;
} Tokenizer;

Tokenizer* Tokenizer_New(String);

void Tokenizer_Delete(Tokenizer*);

void Tokenizer_Tokenize(Tokenizer*);

#endif