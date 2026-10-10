#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "parser.h"
#include "shell.h"
#include "tokenizer.h"

// "<"
static int handel_input_redirect(struct token *tokens, int count) {
  int redirect_index = -1;

  for (int i = 0; i < count; i++) {
    if (tokens[i].type == TOKEN_REDIR_IN) {
      if (redirect_index != -1) {
        printf("Invalid input redirection!\n");
        return 1;
      }

      redirect_index = i;
    }
  }

  if (redirect_index == -1) {
    return 0;
  }

  if (redirect_index == 0 || redirect_index + 2 != count ||
      tokens[redirect_index + 1].type != TOKEN_WORD) {
    printf("Invalid input redirection!\n");
    return 1;
  }

  char *args[64];
  int argc = 0;

  for (int i = 0; i < redirect_index; i++) {
    if (tokens[i].type != TOKEN_WORD) {
      printf("Invalid input redirection!\n");
      return 1;
    }

    args[argc++] = tokens[i].value;
  }

  args[argc] = NULL;

  execute_input_redirect(args, tokens[redirect_index + 1].value);
  return 1;
}

// '>'
static int handel_output_redirect(struct token *tokens, int count) {
  int redirect_index = -1;

  for (int i = 0; i < count; i++) {
    if (tokens[i].type == TOKEN_REDIR_OUT ||
        tokens[i].type == TOKEN_REDIR_APPEND) {
      if (redirect_index != -1) {
        return 0;
      }

      redirect_index = i;
    }
  }

  if (redirect_index == -1) {
    return 0;
  }

  if (redirect_index == 0 || redirect_index + 2 != count ||
      tokens[redirect_index + 1].type != TOKEN_WORD) {
    printf("Invalid putput redirection!\n");
    return 1;
  }

  char *args[64];
  int argc = 0;

  for (int i = 0; i < redirect_index; i++) {
    if (tokens[i].type != TOKEN_WORD) {
      printf("Invalid output redirection");
      return 1;
    }

    args[argc++] = tokens[i].value;
  }

  args[argc] = NULL;

  int append = tokens[redirect_index].type == TOKEN_REDIR_APPEND;
  execute_redirect(args, tokens[redirect_index + 1].value, append);

  return 1;
}

// tokens -> args
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

// Run the shell
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

      continue;
      // output redirection before the way we handel normal commandsntinue; //
      // we dont execute it as normal command now ofc
    }

    // output redirection before the way we handel normal commands
    if (handel_output_redirect(tokens, count)) {
      continue;
    }

    // input redirection
    if (handel_input_redirect(tokens, count)) {
      continue;
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
