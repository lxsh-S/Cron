#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "parser.h"
#include "shell.h"
#include "tokenizer.h"

void shell_run(void) {
  char input[1024];

  while (1) {
    printf("λ ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      break;
    }

    if (input[0] == '\0') {
      continue;
    }

    struct token tokens[64];

    int count = tokenizer(input, tokens);

    for (int i = 0; i < count; i++) {
      printf("TYPE: %d | VALUE: %s\n", tokens[i].type, tokens[i].value);
    }
  }
}
