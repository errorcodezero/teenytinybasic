#include "scanner.h"
#include "basic.h"
#include <stddef.h>
#include <string.h>

Basic scan(char *source) {
  Basic basic = {
      .source = source,
	  .length = strlen(source)
  };

  return basic;
}
