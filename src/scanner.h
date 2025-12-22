#ifndef INCLUDE_SRC_SCANNER_H_
#define INCLUDE_SRC_SCANNER_H_

#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

typedef struct Scanner {
  wchar_t *source;
  size_t length;
  size_t index;
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
  END
} Token;

void scan(wchar_t *source);
wchar_t advance(Scanner *scanner);
bool at_end(Scanner *scanner);
wchar_t peek(Scanner *scanner);

#endif // INCLUDE_SRC_SCANNER_H_
