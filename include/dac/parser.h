#ifndef PARSER_H
#define PARSER_H

#include "dac/lexer.h"
#include "dac/node.h"
#include "dac/string.h"
#include "dac/vector.h"

typedef struct {
  Vector* tokens;
  unsigned int __pos__;
} Parser;

Parser* Parser_New(Vector*);

void Parser_Delete(Parser*);

Token* Parser_Consume(Parser*, TokenType);

Node* Parser_ParseStatement(Parser*);

Node* Parser_ParseFunction(Parser*);

NodeVector* Parser_ParseProgram(Parser*);

#endif