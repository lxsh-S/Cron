#include "parser.h"
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

  // Found redirect
  *redirect_pos = '\0';

  *command = input;
  *file = redirect_pos + 1;

  return 1;
}
