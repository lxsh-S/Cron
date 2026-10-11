#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"

int handel_builtin(char **args) {
  if (args[0] == NULL) {
    return 1;
  }

  if (strcmp(args[0], "cd") == 0) {
    char *directory = args[1];

    if (directory == NULL) {
      directory = getenv("HOME");
    }

    if (directory == NULL || chdir(directory) == -1) {
      perror("cd");
    }

    return 1;
  }

  if (strcmp(args[0], "help") == 0) {
    printf("Cron shell!!!! -- Built in commands:\n");
    printf("cd [directory] -- Change the working directory\n");
    printf("help           -- Show this help message :)\n");
    printf("exit           -- Exit Cron\n");
    printf("----------------------------------------------\n");
    printf("Pipes: | Input: < Output: > Append: >>\n");
    return 1;
  }

  return 0;
}
