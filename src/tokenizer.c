#include "tokenizer.h"
#include <string.h>

int tokenizer(char *input, struct token *tokens) {
  int count = 0;
  char *start = input;

  while (*start != '\0') {
    // we skip spaces
    if (*start == ' ') {
      start++;
      continue;
    }

    // Pipe
    if (*start == '|') {
      tokens[count].type = TOKEN_PIPE;
      tokens[count].value = start;

      start++;
      count++;
      continue;
    }

    // Word
    char *work_start = start;

    while (*start != '\0' && *start != ' ' && *start != '|') {
      start++;
    }

    if (*start == '|') {
      *start = '\0';
      start++;
    } else if (*start == ' ') {
      *start = '\0';
      start++;
    }

    tokens[count].type = TOKEN_WORD;
    tokens[count].value = work_start;

    count++;
  }
  return count; // returning the number of tokens we created
}
