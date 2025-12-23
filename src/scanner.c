#include "scanner.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define SINGLE_CHARACTER_SCAN(SCANNER, CHARACTER, CASE, TOKEN)                 \
  if (CHARACTER == CASE) {                                                     \
    push_token(&SCANNER, TOKEN);                                               \
    continue;                                                                  \
  }

#define DOUBLE_CHARACTER_SCAN(SCANNER, CHARACTER, CASE, TOKEN)                 \
  if (CHARACTER == CASE[0]) {                                                  \
    bool matches = advance(&SCANNER) == CASE[1];                               \
    if (matches) {                                                             \
      push_token(&SCANNER, TOKEN);                                             \
      continue;                                                                \
    } else                                                                     \
      back(&SCANNER);                                                          \
  }

TokensList *scan(wchar_t *source) {
  Scanner scanner = {
      .source = source, .index = 0, .length = wcslen(source), .tokens = NULL};

  while (!at_end(&scanner)) {
    wchar_t character = advance(&scanner);
    SINGLE_CHARACTER_SCAN(scanner, character, '+', PLUS)
    SINGLE_CHARACTER_SCAN(scanner, character, '-', MINUS)
    SINGLE_CHARACTER_SCAN(scanner, character, '(', LEFT_PAREN)
    SINGLE_CHARACTER_SCAN(scanner, character, ')', RIGHT_PAREN)
    SINGLE_CHARACTER_SCAN(scanner, character, '=', EQUAL)
    DOUBLE_CHARACTER_SCAN(scanner, character, L"<>", NOT_EQUAL)
    DOUBLE_CHARACTER_SCAN(scanner, character, L"<=", LESS_THAN_EQUAL)
    SINGLE_CHARACTER_SCAN(scanner, character, '<', LESS_THAN)
    DOUBLE_CHARACTER_SCAN(scanner, character, L">=", GREATER_THAN_EQUAL)
    SINGLE_CHARACTER_SCAN(scanner, character, '>', GREATER_THAN)
  }

  return scanner.tokens;
}

bool at_end(Scanner *scanner) { return (scanner->index >= scanner->length); }

wchar_t advance(Scanner *scanner) {
  if (!at_end(scanner)) {
    wchar_t character = scanner->source[scanner->index++];
    return character;
  }
  return '\0';
}

wchar_t back(Scanner *scanner) {
  if (scanner->index > 0) {
    return scanner->source[--scanner->index];
  }
  return '\0';
}

wchar_t peek(Scanner *scanner) { return scanner->source[scanner->index]; }

void push_token(Scanner *scanner, Token token) {
  // if the tokens list hasn't been instantiated yet
  if (scanner->tokens == NULL) {
    scanner->tokens = calloc(1, sizeof(TokensList));
    Token *list = calloc(1, sizeof(Token));
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
