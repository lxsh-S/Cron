#include "executor.h"

#include <fcntl.h>
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

// We execute the pipeline here when '|' ofc
void execute_pipeline(char **left_args, char **right_args) {
  int pipefd[2];

  pipe(pipefd);

  pid_t left_pid = fork();

  if (left_pid == 0) {
    close(pipefd[0]);

    dup2(pipefd[1], STDOUT_FILENO);

    close(pipefd[1]);

    execvp(left_args[0], left_args);

    // We knoW that this will only run if the 'exec' fails
    perror(left_args[0]);
    exit(1);
  }

  pid_t right_pid = fork();

  if (right_pid == 0) {
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO);

    close(pipefd[0]);

    execvp(right_args[0], right_args);

    perror(right_args[0]);
    exit(1);
  }

  // Cron is the parent of these two childs above (!!!Wow!!!)
  close(pipefd[0]);
  close(pipefd[1]);

  waitpid(left_pid, NULL, 0);
  waitpid(right_pid, NULL, 0);
}

// Execute rediret to file
void execute_redirect(char **args, char *file, int append) {
  pid_t pid = fork();

  if (pid == 0) {
    int flags = O_WRONLY | O_CREAT;

    if (append) {
      flags |= O_APPEND;
    } else {
      flags |= O_TRUNC;
    }

    int fd = open(file, flags, 0644);

    // If error
    if (fd == -1) {
      perror("file");
      exit(1);
    }

    dup2(fd, STDOUT_FILENO);
    close(fd);

    execvp(args[0], args);

    perror(args[0]);
    exit(1);
  }

  waitpid(pid, NULL, 0);
}

void execute_input_redirect(char **args, char *file) {
  pid_t pid = fork();

  if (pid == 0) {
    int fd = open(file, O_RDONLY);

    if (fd == -1) {
      perror(file);
      exit(1);
    }

    dup2(fd, STDIN_FILENO);
    close(fd);

    execvp(args[0], args);

    perror(args[0]);
    exit(1);
  }

  waitpid(pid, NULL, 0);
}
