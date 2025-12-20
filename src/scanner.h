#ifndef INCLUDE_SRC_SCANNER_H_
#define INCLUDE_SRC_SCANNER_H_

#include "basic.h"

typedef struct Scanner {
  char *source;
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

Basic scan(char *source);

#endif // INCLUDE_SRC_SCANNER_H_
