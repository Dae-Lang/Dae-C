#include "dac/parser.h"

#include <stdint.h>
#include <stdlib.h>

#include "dac/bool.h"
#include "dac/error.h"
#include "dac/lexer.h"
#include "dac/node.h"
#include "dac/string.h"
#include "dac/vector.h"

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
    Error_Fatal("Expected %d, but got %d\n", exType, token->type);
    return NULL;
  }
  parser->__pos__++;
  return token;
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
      Error_Fatal(
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
      Bool rawBool = false;
      String boolStr = Parser_Consume(parser, TOKEN_BOOLEAN)->text;
      if (String_Equals(boolStr, "true"))
        rawBool = true;
      value = (void*)(intptr_t)rawBool;
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
    Parser_Consume(parser, TOKEN_LPAREN);
    Parser_Consume(parser, TOKEN_RPAREN);
    return CallNode_New(name);
  }
  Error_Fatal("Unknown statement: %s\n", token->text);
  return NULL;
}

Node* Parser_ParseFunction(Parser* parser) {
  Parser_Consume(parser, TOKEN_KEYWORD);
  String name = Parser_Consume(parser, TOKEN_IDENTIFIER)->text;

  Parser_Consume(parser, TOKEN_LPAREN);
  Parser_Consume(parser, TOKEN_RPAREN);

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
  return FunctionNode_New(name, returnType, body);
}

NodeVector* Parser_ParseProgram(Parser* parser) {
  NodeVector* functions = Vector_New(sizeof(Node));
  do {
    Vector_PushBack(functions, Parser_ParseFunction(parser));
  } while (((Token*)Vector_Get(parser->tokens, parser->__pos__))->type !=
           TOKEN_EOF);
  return functions;
}