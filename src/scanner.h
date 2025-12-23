#ifndef INCLUDE_SRC_SCANNER_H_
#define INCLUDE_SRC_SCANNER_H_

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

typedef struct TokensList TokensList;

typedef struct Scanner {
  wchar_t *source;
  size_t length;
  size_t index;
  TokensList *tokens;
} Scanner;

typedef enum Token {
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
  OR
} Token;

typedef struct TokensList {
  Token *list;
  size_t length;
  size_t capacity;
} TokensList;

TokensList *scan(wchar_t *source);
wchar_t advance(Scanner *scanner);
wchar_t back(Scanner *scanner);
bool at_end(Scanner *scanner);
wchar_t peek(Scanner *scanner);
void push_token(Scanner *scanner, Token token);

#endif // INCLUDE_SRC_SCANNER_H_
