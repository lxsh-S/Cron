#include "parser.h"
#include "tokenizer.h"
#include <string.h>

void parse_command(char *input, char **args) {
  int argc = 0;

  char *token = strtok(input, " ");

  while (token != NULL && argc < 63) {
    args[argc] = token;
    argc++;

    token = strtok(NULL, " ");
  }

  args[argc] = NULL;
}

int split_pipe(char *input, char **left, char **right) {
  char *pipe_pos = strchr(input, '|');

  if (pipe_pos == NULL) {
    return 0;
  }

  *pipe_pos = '\0';

  *left = input;
  *right = pipe_pos + 1;

  return 1;
}

// I/O redirection
int split_redirect(char *input, char **command, char **file) {
  char *redirect_pos = strchr(input, '>');

  // Couldnt find/No redirect
  if (redirect_pos == NULL) {
    return 0;
  }

  // '>>'
  if (*(redirect_pos + 1) == '>') {
    *redirect_pos = '\0';

    *command = input;
    *file = redirect_pos + 2;

    return 2;
  }

  // Found redirect - '>'
  *redirect_pos = '\0';

  *command = input;
  *file = redirect_pos + 1;

  return 1;
}

// I/O redirection '<'
int split_input_redirect(char *input, char **command, char **file) {
  char *redirect_pos = strchr(input, '<');

  if (redirect_pos == NULL) {
    return 0;
  }

  *redirect_pos = '\0';

  *command = input;
  *file = redirect_pos + 1;

  return 1;
}

// tokenizer
int split_tokens_pipe(struct token *tokens, int count, char **left_args,
                      char **right_args) {
  int left_count = 0;
  int right_count = 0;
  int found_pipe = 0;

  for (int i = 0; i < count; i++) {
    if (tokens[i].type == TOKEN_PIPE) {
      found_pipe = 1;
      continue;
    }

    if (tokens[i].type != TOKEN_WORD) {
      return 0;
    }

    if (found_pipe == 0) {
      left_args[left_count] = tokens[i].value;
      left_count++;
    } else {
      right_args[right_count] = tokens[i].value;
      right_count++;
    }
  }

  // Add null at the end
  left_args[left_count] = NULL;
  right_args[right_count] = NULL;

  if (found_pipe == 0 || left_count == 0 || right_count == 0) {
    return 0;
  }

  return 1;
}
