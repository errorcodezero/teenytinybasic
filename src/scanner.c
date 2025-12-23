#include "scanner.h"
#include "parser.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define SINGLE_CHARACTER_SCAN(SCANNER, CHARACTER, CASE, TOKEN, LINE)           \
  if (CHARACTER == CASE) {                                                     \
    push_token(&SCANNER, TOKEN, LINE);                                         \
    continue;                                                                  \
  }

#define DOUBLE_CHARACTER_SCAN(SCANNER, CHARACTER, CASE, TOKEN, LINE)           \
  if (CHARACTER == CASE[0]) {                                                  \
    bool matches = advance(&SCANNER) == CASE[1];                               \
    if (matches) {                                                             \
      push_token(&SCANNER, TOKEN, LINE);                                       \
      continue;                                                                \
    } else                                                                     \
      back(&SCANNER);                                                          \
  }

void scan(wchar_t *source, Parser *parser) {
  Scanner scanner = {.source = source,
                     .index = 0,
                     .length = wcslen(source),
                     .tokens = {.length = 0, .capacity = 0, .list = NULL}};

  size_t line = 1;

  while (!at_end(&scanner)) {
    wchar_t character = advance(&scanner);
    SINGLE_CHARACTER_SCAN(scanner, character, '+', PLUS, line)
    SINGLE_CHARACTER_SCAN(scanner, character, '-', MINUS, line)
    SINGLE_CHARACTER_SCAN(scanner, character, '(', LEFT_PAREN, line)
    SINGLE_CHARACTER_SCAN(scanner, character, ')', RIGHT_PAREN, line)
    SINGLE_CHARACTER_SCAN(scanner, character, '=', EQUAL, line)
    DOUBLE_CHARACTER_SCAN(scanner, character, L"<>", NOT_EQUAL, line)
    DOUBLE_CHARACTER_SCAN(scanner, character, L"<=", LESS_THAN_EQUAL, line)
    SINGLE_CHARACTER_SCAN(scanner, character, '<', LESS_THAN, line)
    DOUBLE_CHARACTER_SCAN(scanner, character, L">=", GREATER_THAN_EQUAL, line)
    SINGLE_CHARACTER_SCAN(scanner, character, '>', GREATER_THAN, line)
    SINGLE_CHARACTER_SCAN(scanner, character, '*', STAR, line)
    // newline
    if (character == L'\n') {
      line++;
    }
    // number parsing
    if ('0' <= character && character <= '9') {
      wchar_t *number = malloc(sizeof(wchar_t) * 2);
      size_t capacity = 2;
      size_t length = 2;
      number[0] = character;
      number[1] = '\0';
      character = advance(&scanner);
      while (character == '.' || ('0' <= character && character <= '9')) {
        if (length >= capacity) {
          capacity *= 2;
          capacity++;
          number = realloc(number, sizeof(wchar_t) * capacity);
        }
        number[length - 1] = character;
        number[length++] = '\0';
        character = advance(&scanner);
      }
      wchar_t *number_fit = malloc(sizeof(wchar_t) * length);
      wcscpy(number_fit, number);
      free(number);
      number = NULL;
      push_token_with_data(&scanner, NUMBER, line, number_fit);
      if (('0' > character || character > '9') && character != '\0') {
        back(&scanner);
      }
    }
  }

  parser->tokens.list = scanner.tokens.list;
  parser->tokens.capacity = scanner.tokens.capacity;
  parser->tokens.length = scanner.tokens.length;
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
    wchar_t character = scanner->source[--scanner->index];
    return character;
  }
  return '\0';
}

wchar_t peek(Scanner *scanner) { return scanner->source[scanner->index]; }

void push_token(Scanner *scanner, TokenType token, size_t line) {
  // if the tokens list hasn't been instantiated yet
  if (scanner->tokens.length == 0) {
    Token *list = calloc(1, sizeof(Token));
    scanner->tokens.capacity = 1;
    scanner->tokens.list = list;
  }
  if (scanner->tokens.length >= scanner->tokens.capacity) {
    scanner->tokens.capacity *= 2;
    scanner->tokens.list =
        realloc(scanner->tokens.list, sizeof(Token) * scanner->tokens.capacity);
  }
  scanner->tokens.list[scanner->tokens.length].token_type = token;
  scanner->tokens.list[scanner->tokens.length].line = line;
  scanner->tokens.list[scanner->tokens.length++].data = NULL;
}

void push_token_with_data(Scanner *scanner, TokenType token, size_t line,
                          wchar_t *data) {
  push_token(scanner, token, line);
  scanner->tokens.list[scanner->tokens.length - 1].data = data;
}
