#include "tokenizer.h"
#include <stdio.h>
int main(void) {
  char input[1024];

  printf("> ");
  fgets(input, sizeof(input), stdin);
  printf("INPUT: [%s]\n", input);

  struct token tokens[64];

  int count = tokenizer(input, tokens);

  for (int i = 0; i < count; i++) {
    printf("TYPE: %d | VALUE: %s\n", tokens[i].type, tokens[i].value);
  }

  return 0;
}
