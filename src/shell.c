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
      char *left_args[64];
      char *right_args[64];

      parse_command(left, left_args);
      parse_command(right, right_args);

      printf("LEFT COMMAND: %s\n", left_args[0]);
      printf("RIGHT COMMAND: %s\n", right_args[0]);
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
