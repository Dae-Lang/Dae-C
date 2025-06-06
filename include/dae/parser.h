#ifndef PARSER_H
#define PARSER_H

#include <stdarg.h>

#include "dae/lexer.h"
#include "dae/node.h"
#include "dae/string.h"
#include "dae/vector.h"

typedef struct {
  Vector* tokens;
  NodeVector* functions;
  unsigned int __pos__;
} Parser;

Parser* Parser_New(Vector*);

void Parser_Delete(Parser*);

Token* Parser_Consume(Parser*, TokenType);

Node* Parser_FindFunction(Parser*, String);

Node* Parser_ParseFunction(Parser*);

Node* Parser_ParseStatement(Parser*);

NodeVector* Parser_ParseProgram(Parser*);

void Parser_Error(String, ...);

#endif