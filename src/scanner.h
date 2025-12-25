#ifndef INCLUDE_SRC_SCANNER_H_
#define INCLUDE_SRC_SCANNER_H_

#include "basic.h"
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

typedef struct Parser Parser;
typedef struct Token Token;

typedef struct TokensList {
  Token *list;
  size_t length;
  size_t capacity;
} TokensList;

typedef struct Scanner {
  wchar_t *source;
  size_t length;
  size_t index;
  TokensList tokens;
} Scanner;

typedef enum TokenType {
  PRINT,
  IF,
  THEN,
  GOTO,
  INPUT,
  LET,
  GOSUB,
  RETURN,
  CLEAR,
  LIST,
  RUN,
  END,
  LEFT_PAREN,
  RIGHT_PAREN,
  PLUS,
  MINUS,
  SLASH,
  LESS_THAN,
  LESS_THAN_EQUAL,
  GREATER_THAN,
  GREATER_THAN_EQUAL,
  STAR,
  NOT_EQUAL,
  EQUAL,
  AND,
  NOT,
  OR,
  NUMBER,
  IDENTIFIER,
  STRING,
  NEW_LINE,
  END_OF_FILE,
} TokenType;

typedef struct Token {
  TokenType token_type;
  size_t line;
  wchar_t *data;
} Token;

void scan(wchar_t *source, Parser *parser);
wchar_t advance(Scanner *scanner);
wchar_t back(Scanner *scanner);
bool at_end(Scanner *scanner);
wchar_t peek(Scanner *scanner);
void push_token(Scanner *scanner, TokenType token, size_t line);
void push_token_with_data(Scanner *scanner, TokenType token, size_t line,
                          wchar_t *data);

#endif // INCLUDE_SRC_SCANNER_H_
