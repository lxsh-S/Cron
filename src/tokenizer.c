#include "tokenizer.h"
#include <string.h>
#include <threads.h>

int tokenizer(char *input, struct token *tokens) {
  int count = 0;
  char *p = input;

  while (*p != '\0') {

    // Ofc we skip spaces
    if (*p == ' ') {
      p++;
      continue;
    }

    // Pipe
    if (*p == '|') {
      tokens[count].type = TOKEN_PIPE;
      strcpy(tokens[count].value, "|");

      p++;
      count++;
      continue;
    }

    // Word
    char *start = p;

    while (*p != '\0' && *p != ' ' && *p != '|') {
      p++;
    }

    int length = p - start;

    strncpy(tokens[count].value, start, length);
    tokens[count].value[length] = '\0';

    tokens[count].type = TOKEN_WORD;
    count++;
  }
  return count;
}
