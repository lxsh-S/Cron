#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "parser.h"
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

    int has_pipe = 0;

    for (int i = 0; i < count; i++) {
      if (tokens[i].type == TOKEN_PIPE) {
        has_pipe = 1;
        break;
      }
    }

    if (has_pipe) {
      char *left_args[64];
      char *right_args[64];

      if (split_tokens_pipe(tokens, count, left_args, right_args)) {
        execute_pipeline(left_args, right_args);
      } else {
        printf("Invalid pipe commad!\n");
      }

      continue; // we dont execute it as normal command now ofc
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
