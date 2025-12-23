#include "scanner.h"
#include <wchar.h>

int main() {
  wchar_t *text = L"(><<><=)>==<<=";
  scan(text);
}
