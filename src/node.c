#include "dac/node.h"

#include <stdlib.h>

#include "dac/string.h"
#include "dac/vector.h"

Node* FunctionNode_New(const String name,
                       const String returnType,
                       Vector* body) {
  Node* node = malloc(sizeof(Node));
  node->type = NODE_FUNCTION;
  node->function_n.name = name;
  node->function_n.returnType = returnType;
  node->function_n.body = body;
  return node;
}

Node* PrintNode_New(NodePrintMessageType type, const String message) {
  Node* node = malloc(sizeof(Node));
  node->type = NODE_PRINT;
  node->print_n.message = strdup(message);
  node->print_n.printMessageType = type;
  return node;
}

Node* CallNode_New(const String functionName) {
  Node* node = malloc(sizeof(Node));
  node->type = NODE_CALL;
  node->call_n.functionName = functionName;
  return node;
}

Node* ReturnNode_New(NodeReturnType returnType, void* returnValue) {
  Node* node = malloc(sizeof(Node));
  node->type = NODE_RETURN;
  node->return_n.returnType = returnType;
  node->return_n.returnValue = returnValue;
  return node;
}