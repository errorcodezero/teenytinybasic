#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

int main() {
  wchar_t *text = L"hello world";
  scan(text);
}
