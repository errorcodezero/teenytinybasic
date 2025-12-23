#include "parser.h"
#include "scanner.h"
#include <stdlib.h>
#include <wchar.h>

int main() {
  // wchar_t *text = L"(-123.456) + 324 * 4 - 5 >= 4";
  wchar_t *text = L"12323123123123";
  Parser parser = {.tokens = {.list = NULL, .length = 0, .capacity = 0}};
  scan(text, &parser);

  for (size_t i = 0; i < parser.tokens.length; i++) {
    if (parser.tokens.list[0].data != NULL) {
      free(parser.tokens.list[0].data);
      parser.tokens.list[0].data = NULL;
    }
  }

  free(parser.tokens.list);
  parser.tokens.list = NULL;
}
