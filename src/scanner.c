#include "scanner.h"
#include "parser.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define SINGLE_CHARACTER_SCAN(CASE, TOKEN)                                     \
  if (character == CASE) {                                                     \
    push_token(&scanner, TOKEN, line);                                         \
    continue;                                                                  \
  }

#define DOUBLE_CHARACTER_SCAN(CASE, TOKEN)                                     \
  if (character == CASE[0]) {                                                  \
    bool matches = advance(&scanner) == CASE[1];                               \
    if (matches) {                                                             \
      push_token(&scanner, TOKEN, line);                                       \
      continue;                                                                \
    } else                                                                     \
      back(&scanner);                                                          \
  }

#define KEYWORD(WORD, TOKEN)                                                   \
  if (!wcscmp(WORD, identifier_fit)) {                                         \
    flag = true;                                                               \
    push_token(&scanner, TOKEN, line);                                         \
    continue;                                                                  \
  }

void scan(wchar_t *source, Parser *parser) {
  Scanner scanner = {.source = source,
                     .index = 0,
                     .length = wcslen(source),
                     .tokens = {.length = 0, .capacity = 0, .list = NULL}};

  size_t line = 1;

  while (!at_end(&scanner)) {
    wchar_t character = advance(&scanner);
    SINGLE_CHARACTER_SCAN('+', PLUS)
    SINGLE_CHARACTER_SCAN('-', MINUS)
    SINGLE_CHARACTER_SCAN('(', LEFT_PAREN)
    SINGLE_CHARACTER_SCAN(')', RIGHT_PAREN)
    SINGLE_CHARACTER_SCAN('=', EQUAL)
    DOUBLE_CHARACTER_SCAN(L"<>", NOT_EQUAL)
    DOUBLE_CHARACTER_SCAN(L"<=", LESS_THAN_EQUAL)
    SINGLE_CHARACTER_SCAN('<', LESS_THAN)
    DOUBLE_CHARACTER_SCAN(L">=", GREATER_THAN_EQUAL)
    SINGLE_CHARACTER_SCAN('>', GREATER_THAN)
    SINGLE_CHARACTER_SCAN('*', STAR)
    if (character == L' ')
      continue;
    // newline
    if (character == L'\n') {
      push_token(&scanner, NEWLINE, line++);
      line++;
      continue;
    }
    // string
    if (character == '"') {
      bool backslashed = false;
      wchar_t *string = malloc(sizeof(wchar_t) * 2);
      size_t capacity = 1;
      size_t length = 1;
      string[0] = '\0';
      character = advance(&scanner);
      while (character != '"' || backslashed) {
        if (length >= capacity) {
          capacity *= 2;
          capacity++;
          string = realloc(string, sizeof(wchar_t) * capacity);
        }
        string[length - 1] = character;
        string[length++] = '\0';
        if (character == '\\') {
          backslashed = true;
        } else {
          backslashed = false;
        }
        character = advance(&scanner);
      }
      wchar_t *string_fit = malloc(sizeof(wchar_t) * length);
      wcscpy(string_fit, string);
      free(string);
      string = NULL;
      push_token_with_data(&scanner, STRING, line, string_fit);
    }
    // number parsing
    else if ('0' <= character && character <= '9') {
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
    } else {
      wchar_t *identifier = malloc(sizeof(wchar_t) * 2);
      size_t capacity = 2;
      size_t length = 2;
      identifier[0] = character;
      identifier[1] = '\0';
      character = advance(&scanner);
      while (('a' <= character && character <= 'z') ||
             ('A' <= character && character <= 'Z') ||
             ('0' <= character && character <= '9')) {
        if (length >= capacity) {
          capacity *= 2;
          capacity++;
          identifier = realloc(identifier, sizeof(wchar_t) * capacity);
        }
        identifier[length - 1] = character;
        identifier[length++] = '\0';
        character = advance(&scanner);
      }
      wchar_t *identifier_fit = malloc(sizeof(wchar_t) * length);
      wcscpy(identifier_fit, identifier);
      free(identifier);
      identifier = NULL;
      bool flag = false;
      KEYWORD(L"OR", OR);
      KEYWORD(L"AND", AND);
      KEYWORD(L"NOT", NOT);
      KEYWORD(L"GOTO", GOTO);
      KEYWORD(L"PRINT", PRINT);
      KEYWORD(L"IF", IF);
      KEYWORD(L"THEN", THEN);
      KEYWORD(L"INPUT", INPUT);
      KEYWORD(L"LET", LET);
      KEYWORD(L"GOSUB", GOSUB);
      KEYWORD(L"RETURN", RETURN);
      KEYWORD(L"CLEAR", CLEAR);
      KEYWORD(L"LIST", LIST);
      KEYWORD(L"RUN", RUN);
      KEYWORD(L"END", END);
      if (!flag) {
        push_token_with_data(&scanner, IDENTIFIER, line, identifier_fit);
      }
      if (('a' > character || character > 'z') &&
          ('A' > character || character > 'Z') && character != '\0') {
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
