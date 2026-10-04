#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_command(char **args) {
  pid_t pid = fork();

  if (pid == 0) {
    execvp(args[0], args);

    perror(args[0]);
    exit(1);
  }

  waitpid(pid, NULL, 0);
}
