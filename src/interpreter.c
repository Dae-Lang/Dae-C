#include "dae/interpreter.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "dae/error.h"
#include "dae/hashmap.h"
#include "dae/node.h"

Interpreter* Interpreter_New(NodeVector* nodes) {
  assert(nodes);
  Interpreter* interpreter = malloc(sizeof(Interpreter));
  interpreter->nodes = nodes;
  interpreter->functions = HashMap_New(sizeof(Node*));

  // register all funcs
  for (int i = 0; i < nodes->size; i++) {
    Node* node = (Node*)Vector_Get(nodes, i);
    if (node->type == NODE_FUNCTION) {
      HashMap_Put(interpreter->functions, node->function_n.name, &node);
    }
  }

  return interpreter;
}

void Interpreter_Delete(Interpreter* self) {
  // maybe desallocate nodes?
  free(self);
}

InterpreterResult Interpreter_Run(Interpreter* self) {
  assert(self);

  Node* main = *(Node**)HashMap_Get(self->functions, "main");
  if (main == NULL) {
    Error_Fatal("Your program needs a main function!\n");
  }

  return Interpreter_RunFunc(self, main);
}

InterpreterResult Interpreter_RunFunc(Interpreter* self, Node* func) {
  assert(self);
  assert(func);
  assert(func->type == NODE_FUNCTION);

  for (int i = 0; i < func->function_n.body->size; i++) {
    Node* stmt = (Node*)Vector_Get(func->function_n.body, i);
    InterpreterResult result = Interpreter_RunNode(self, stmt);
    if (result.type == IRT_RETURN) {
      return result;
    }
  }

  // default value
  InterpreterResult ret = {.type = IRT_RETURN, .data = NULL};
  return ret;
}

InterpreterResult Interpreter_RunNode(Interpreter* self, Node* node) {
  assert(self);
  assert(node);

  switch (node->type) {
    case NODE_PRINT:
      printf("%s\n", node->print_n.message);
      return (InterpreterResult){.type = IRT_NONE, .data = NULL};

    case NODE_CALL: {
      Node* called =
          *(Node**)HashMap_Get(self->functions, node->call_n.functionName);
      if (called == NULL) {
        Error_Fatal("Function not found: %s\n", node->call_n.functionName);
      }
      return Interpreter_RunFunc(self, called);
    }

    case NODE_RETURN: {
      return (InterpreterResult){.type = IRT_RETURN,
                                 .data = node->return_n.returnValue};
    }

    default:
      Error_Fatal("Unknown node type %d\n", node->type);
  }
  return (InterpreterResult){.type = IRT_RETURN, .data = NULL};
}