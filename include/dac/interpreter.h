#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "dac/hashmap.h"
#include "dac/node.h"
#include "dac/string.h"
#include "dac/vector.h"

typedef struct {
  NodeVector* nodes;
  HashMap* functions;
} Interpreter;

typedef enum { IRT_NONE, IRT_RETURN } InterpreterResultType;

typedef struct {
  void* data;
  InterpreterResultType type;
} InterpreterResult;

Interpreter* Interpreter_New(NodeVector* nodes);

void Interpreter_Delete(Interpreter* interpreter);

InterpreterResult Interpreter_Run(Interpreter* interpreter);

InterpreterResult Interpreter_RunFunc(Interpreter* interpreter, Node* func);

InterpreterResult Interpreter_RunNode(Interpreter* interpreter, Node* node);

#endif