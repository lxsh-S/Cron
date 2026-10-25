#include "expansion.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
// read envs
char *expand_variable(char *word) {
  if (word[0] != '$') {
    return word;
  }

  char *slash = strchr(word, '/');

  if (slash == NULL) {
    char *value = getenv(word + 1);

    if (value == NULL) {
      return "";
    }

    return value;
  }

  size_t name_length = (size_t)(slash - word - 1);
  char name[64];

  if (name_length >= sizeof(name)) {
    return "";
  }

  memcpy(name, word + 1, name_length);
  name[name_length] = '\0';

  char *value = getenv(name);

  if (value == NULL) {
    return "";
  }

  size_t value_lenght = strlen(value);
  size_t path_length = strlen(slash);

  char *result = malloc(value_lenght + path_length + 1);

  if (result == NULL) {
    return "";
  }

  strcpy(result, value);
  strcat(result, slash);

  return result;
}
