#include "tokenizer.h"
#include <string.h>

int tokenizer(char *input, struct token *tokens) {
  int count = 0;

  char *token = strtok(input, " ");

  while (token != NULL) {
    tokens[count].type = TOKEN_WORD;
    tokens[count].value = token;

    count++;

    token = strtok(NULL, " ");
  }
  return count;
}
