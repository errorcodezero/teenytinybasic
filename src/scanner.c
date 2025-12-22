#include "scanner.h"
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

void scan(wchar_t *source) {
  Scanner scanner = {.source = source, .index = 0, .length = wcslen(source)};

  while (!at_end(&scanner)) {
    wchar_t character = advance(&scanner);
    switch (character) {
    }
    wprintf(L"%c", character);
  }
}

bool at_end(Scanner *scanner) { return (scanner->index == scanner->length); }

wchar_t advance(Scanner *scanner) {
  if (!at_end(scanner)) {
    return scanner->source[scanner->index++];
  }
  return '\0';
}

wchar_t peek(Scanner *scanner) { return scanner->source[scanner->index]; }
