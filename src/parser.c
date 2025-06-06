#include "dae/parser.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

#include "dae/error.h"
#include "dae/lexer.h"
#include "dae/node.h"
#include "dae/string.h"
#include "dae/vector.h"

Parser* Parser_New(Vector* tokens) {
  Parser* parser = malloc(sizeof(Parser));
  parser->tokens = tokens;
  parser->__pos__ = 0;
  return parser;
}

void Parser_Delete(Parser* parser) {
  free(parser);
}

Token* Parser_Consume(Parser* parser, TokenType exType) {
  Token* token = (Token*)Vector_Get(parser->tokens, parser->__pos__);
  if (token->type != exType) {
    Parser_Error("Expected %d, but got %d", exType, token->type);
    return NULL;
  }
  parser->__pos__++;
  return token;
}

Node* Parser_FindFunction(Parser* parser, String name) {
  for (size_t i = 0; i < parser->functions->size; i++) {
    Node* fn = (Node*)Vector_Get(parser->functions, i);
    if (String_Equals(fn->function_n.name, name)) {
      return fn;
    }
  }
  return NULL;
}

Node* Parser_ParseStatement(Parser* parser) {
  Token* token = (Token*)Vector_Get(parser->tokens, parser->__pos__);
  if (String_Equals(token->text, "print")) {
    Parser_Consume(parser, TOKEN_KEYWORD);
    Parser_Consume(parser, TOKEN_ARROW);

    Token* next = (Token*)Vector_Get(parser->tokens, parser->__pos__);

    NodePrintMessageType type;
    String message;

    if (next->type == TOKEN_BOOLEAN) {
      type = NODE_PRINT_BOOL;
      message = Parser_Consume(parser, TOKEN_BOOLEAN)->text;
    } else if (next->type == TOKEN_STRING) {
      type = NODE_PRINT_STRING;
      message = Parser_Consume(parser, TOKEN_STRING)->text;
    } else if (next->type == TOKEN_NUMBER) {
      type = NODE_PRINT_NUMBER;
      message = Parser_Consume(parser, TOKEN_NUMBER)->text;
    } else {
      Parser_Error(
          "Invalid value after 'print ->'. Expected boolean, string or "
          "number.");
    }

    return PrintNode_New(type, message);
  } else if (String_Equals(token->text, "return")) {
    Parser_Consume(parser, TOKEN_KEYWORD);
    Parser_Consume(parser, TOKEN_ARROW);
    Token* next = (Token*)Vector_Get(parser->tokens, parser->__pos__);
    void* value;
    NodeReturnType type;
    if (next->type == TOKEN_BOOLEAN) {
      bool rawbool = false;
      String boolStr = Parser_Consume(parser, TOKEN_BOOLEAN)->text;
      if (String_Equals(boolStr, "true"))
        rawbool = true;
      value = (void*)(intptr_t)rawbool;
      type = NODE_RETURN_BOOL;
    } else if (next->type == TOKEN_NUMBER) {
      value = (void*)(intptr_t)String_ToInt(
          Parser_Consume(parser, TOKEN_NUMBER)->text);
      type = NODE_RETURN_NUMBER;
    } else if (next->type == TOKEN_STRING) {
      value = Parser_Consume(parser, TOKEN_STRING)->text;
      type = NODE_RETURN_STRING;
    }
    return ReturnNode_New(type, value);
  } else if (token->type == TOKEN_IDENTIFIER) {
    String name = Parser_Consume(parser, TOKEN_IDENTIFIER)->text;
    Token* next = (Token*)Vector_Get(parser->tokens, parser->__pos__);

    Node* fn = Parser_FindFunction(parser, name);
    if (fn == NULL) {
      Parser_Error("Function not declared: %s", name);
    }

    if (next->type == TOKEN_LPAREN) {
      if (fn->function_n.params != NULL) {
        if (fn->function_n.params->size >= 1) {
          printf("Function %s expect argumenets : ", name);
          for (size_t i = 0; i < fn->function_n.params->size; ++i) {
            NodeFunctionParam* param =
                (NodeFunctionParam*)Vector_Get(fn->function_n.params, i);
            printf("%s: %s ", param->type, param->name);
          }
          printf("\n");
          Parser_Error("Missing arguments!");
        }
      }
      Parser_Consume(parser, TOKEN_LPAREN);
      Parser_Consume(parser, TOKEN_RPAREN);
      return CallNode_New(name, NULL);
    } else if (next->type == TOKEN_ARROW) {
      Parser_Consume(parser, TOKEN_ARROW);

      size_t expectedArgs = fn->function_n.params->size;
      Vector* args = Vector_New(sizeof(String));

      for (size_t i = 0; i < expectedArgs; i++) {
        Token* arg = (Token*)Vector_Get(parser->tokens, parser->__pos__);

        NodeFunctionParam* param =
            (NodeFunctionParam*)Vector_Get(fn->function_n.params, i);
        String expectedType = param->type;

        bool typeOk =
            (arg->type == TOKEN_STRING &&
             String_Equals(expectedType, "string")) ||
            (arg->type == TOKEN_BOOLEAN &&
             String_Equals(expectedType, "bool")) ||
            (arg->type == TOKEN_NUMBER && String_Equals(expectedType, "int"));

        if (!typeOk) {
          Parser_Error("Argument %zu to '%s' must be of type %s", i + 1, name,
                       expectedType);
        }

        Vector_PushBack(args, arg->text);
        Parser_Consume(parser, arg->type);

        // verify the comma
        if (i < expectedArgs - 1) {
          Token* comma = (Token*)Vector_Get(parser->tokens, parser->__pos__);
          if (comma->type != TOKEN_COMMA) {
            Parser_Error(
                "Expected ',' between arguments in function call to '%s'",
                name);
          }
          Parser_Consume(parser, TOKEN_COMMA);
        }
      }

      return CallNode_New(name, args);
    }

    Parser_Error("Unexpected token after identifier %s", next->type);
  }
  Parser_Error("Unknown statement: %s", token->text);
  return NULL;
}

Node* Parser_ParseFunction(Parser* parser) {
  Parser_Consume(parser, TOKEN_KEYWORD);
  String name = Parser_Consume(parser, TOKEN_IDENTIFIER)->text;

  Parser_Consume(parser, TOKEN_LPAREN);
  Vector* params = Vector_New(sizeof(NodeFunctionParam));

  while (true) {
    Token* next = (Token*)Vector_Get(parser->tokens, parser->__pos__);
    if (next->type == TOKEN_RPAREN) {
      Parser_Consume(parser, TOKEN_RPAREN);
      break;
    }

    String type = Parser_Consume(parser, TOKEN_TYPE)->text;
    Parser_Consume(parser, TOKEN_COLON);
    String name = Parser_Consume(parser, TOKEN_IDENTIFIER)->text;

    NodeFunctionParam param = {name, type};
    Vector_PushBack(params, &param);

    next = (Token*)Vector_Get(parser->tokens, parser->__pos__);
    if (next->type == TOKEN_COMMA) {
      Parser_Consume(parser, TOKEN_COMMA);
    }
  }

  String returnType = NULL;
  if (((Token*)Vector_Get(parser->tokens, parser->__pos__))->type ==
      TOKEN_COLON) {
    Parser_Consume(parser, TOKEN_COLON);
    returnType = Parser_Consume(parser, TOKEN_TYPE)->text;
    Parser_Consume(parser, TOKEN_ARROW);
  }

  Parser_Consume(parser, TOKEN_LBRACE);
  Vector* body = Vector_New(sizeof(Node));
  while (((Token*)Vector_Get(parser->tokens, parser->__pos__))->type !=
         TOKEN_RBRACE) {
    Vector_PushBack(body, Parser_ParseStatement(parser));
  }

  Parser_Consume(parser, TOKEN_RBRACE);
  return FunctionNode_New(name, returnType, body, params);
}

NodeVector* Parser_ParseProgram(Parser* parser) {
  parser->functions = Vector_New(sizeof(Node));
  do {
    Vector_PushBack(parser->functions, Parser_ParseFunction(parser));
  } while (((Token*)Vector_Get(parser->tokens, parser->__pos__))->type !=
           TOKEN_EOF);
  return parser->functions;
}

void Parser_Error(String fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "[Parsing Error] ");
  vprintf(fmt, args);
  printf("\n");
  va_end(args);
  exit(1);
}