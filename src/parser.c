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
