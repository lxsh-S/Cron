#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "shell.h"
#include "tokenizer.h"

static void tokens_to_args(struct token *tokens, int count, char **args) {
  int argc = 0;

  for (int i = 0; i < count; i++) {
    if (tokens[i].type != TOKEN_WORD) {
      continue;
    }

    args[argc] = tokens[i].value;
    argc++;
  }

  args[argc] = NULL;
}

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

    int has_operator = 0;

    for (int i = 0; i < count; i++) {
      if (tokens[i].type != TOKEN_WORD) {
        has_operator = 1; // 1 - true for us
        break;
      }
    }

    if (has_operator) {
      printf("Cron currently doesnt support operators because of the new "
             "executor!\n");
    }

    char *args[64];
    // Convert tokens to args to be used in execvp
    tokens_to_args(tokens, count, args);

    // execute the command
    if (args[0] != NULL) {
      execute_command(args);
    }
  }
}
