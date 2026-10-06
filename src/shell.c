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

      execute_pipeline(left_args, right_args);

    } else {
      char *command;
      char *file;

      int redirect_type = split_redirect(input, &command, &file);

      if (redirect_type != 0) {
        char *args[64];

        // Remove blank space after '>'
        while (*file == ' ' || *file == '\t') {
          file++;
        }

        parse_command(input, args);
        execute_redirect(args, file, redirect_type == 2);

      } else {
        char *args[64];
        parse_command(input, args);
        execute_command(args);
      }
    }
  }
}
