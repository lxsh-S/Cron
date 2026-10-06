#include <stdio.h>
#include <string.h>

#include "executor.h"
#include "parser.h"
#include "shell.h"

void shell_run(void) {
  char input[1024];

  while (1) {
    printf(">>> ");

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

    char *left;
    char *right;

    if (split_pipe(input, &left, &right)) {
      printf("LEFT: %s\n", left);
      printf("RIGHT: %s\n", right);
    } else {
      char *args[64];
      parse_command(input, args);
      execute_command(args);
    }

    char *args[64];

    parse_command(input, args);
    execute_command(args);
  }
}
