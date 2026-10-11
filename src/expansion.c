#include "expansion.h"
#include <stdlib.h>

// read envs
char *expand_variable(char *word) {
  if (word[0] != '$') {
    return word;
  }

  char *value = getenv(word + 1);

  if (value == NULL) {
    return "";
  }

  return value;
}
