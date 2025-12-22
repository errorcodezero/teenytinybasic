#include "scanner.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

TokensList *scan(wchar_t *source) {
  Scanner scanner = {
      .source = source, .index = 0, .length = wcslen(source), .tokens = NULL};

  while (!at_end(&scanner)) {
    wchar_t character = advance(&scanner);
    switch (character) {
    case L'+': {
      push_token(&scanner, PLUS);
      break;
    }
    case L'-': {
      push_token(&scanner, MINUS);
      break;
    }
    }
    wprintf(L"%c", character);
  }

  return scanner.tokens;
}

bool at_end(Scanner *scanner) { return (scanner->index == scanner->length); }

wchar_t advance(Scanner *scanner) {
  if (!at_end(scanner)) {
    return scanner->source[scanner->index++];
  }
  return '\0';
}

wchar_t peek(Scanner *scanner) { return scanner->source[scanner->index]; }

void push_token(Scanner *scanner, Token token) {
  if (scanner->tokens == NULL) {
    scanner->tokens = calloc(1, sizeof(TokensList));
    Token* list = calloc(1, sizeof(Token));
    scanner->tokens->list = list;
    scanner->tokens->length = 0;
    scanner->tokens->capacity = 1;
  }
  if (scanner->tokens->length >= scanner->tokens->capacity) {
    scanner->tokens->capacity *= 2;
    scanner->tokens->list = realloc(scanner->tokens->list,
                                    sizeof(Token) * scanner->tokens->capacity);
  }
  scanner->tokens->list[scanner->tokens->length++] = token;
}
