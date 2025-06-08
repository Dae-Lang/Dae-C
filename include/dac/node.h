#ifndef NODE_H
#define NODE_H

#include "dac/string.h"
#include "dac/vector.h"

typedef enum { NODE_FUNCTION, NODE_PRINT, NODE_CALL, NODE_RETURN } NodeType;

typedef enum {
  NODE_RETURN_NUMBER,
  NODE_RETURN_STRING,
  NODE_RETURN_BOOL
} NodeReturnType;

typedef enum {
  NODE_PRINT_STRING,
  NODE_PRINT_NUMBER,
  NODE_PRINT_BOOL
} NodePrintMessageType;

typedef struct Node Node;
typedef Vector NodeVector;

struct Node {
  NodeType type;

  struct {
    String name;
    String returnType;
    NodeVector* body;
  } function_n;

  struct {
    NodePrintMessageType printMessageType;
    String message;
  } print_n;

  struct {
    String functionName;
  } call_n;

  struct {
    NodeReturnType returnType;
    void* returnValue;
  } return_n;
};

Node* FunctionNode_New(String, String, Vector*);

Node* PrintNode_New(NodePrintMessageType, String name);

Node* CallNode_New(String);

Node* ReturnNode_New(NodeReturnType, void*);

#endif