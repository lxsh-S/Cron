#include "executor.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_command(char **args) {
  pid_t pid = fork();

  if (pid == -1) {
    perror("Cron: fork");
    return;
  }

  if (pid == 0) {
    execvp(args[0], args);

    // we come here when exec fails
    int error = errno;

    if (error == ENOENT) {
      fprintf(stderr, "Cron: command not found '%s'\n", args[0]);
      _exit(127);
    }

    errno = error;
    perror(args[0]);
    _exit(126);
  }

  int status;

  if (waitpid(pid, &status, 0) == -1) {
    perror("Cron: waitpid");
  }
}

// CURRENTLY UNSED
// We execute the pipeline here when '|' ofc
void execute_pipeline(char **left_args, char **right_args) {
  int pipefd[2];

  if (pipe(pipefd) == -1) {
    perror("Cron: pipe");
    return;
  }

  pid_t left_pid = fork();

  if (left_pid == -1) {
    perror("Cron: fork");
    close(pipefd[0]);
    close(pipefd[1]);
    return;
  }
  if (left_pid == 0) {
    close(pipefd[0]);

    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
      perror("Cron: dup2");
      _exit(126);
    }

    close(pipefd[1]);

    execvp(left_args[0], left_args);

    int error = errno;

    if (error == ENOENT) {
      fprintf(stderr, "Cron: command not fount-> %s\n", left_args[0]);
    } else {
      errno = error;
      perror(left_args[0]);
    }

    _exit(error == ENOENT ? 127 : 126);
  }

  pid_t right_pid = fork();

  if (right_pid == -1) {
    perror("Cron: fork");

    close(pipefd[0]);
    close(pipefd[1]);

    if (waitpid(left_pid, NULL, 0) == -1) {
      perror("Cron: waitpid");
    }

    return;
  }

  if (right_pid == 0) {
    close(pipefd[1]);

    if (dup2(pipefd[0], STDIN_FILENO) == -1) {
      perror("Cron: dup2");
      _exit(126);
    }

    close(pipefd[0]);

    execvp(right_args[0], right_args);

    int error = errno;

    if (error == ENOENT) {
      fprintf(stderr, "Cron: command not found-> %s\n", right_args[0]);
    } else {
      errno = error;
      perror(right_args[0]);
    }

    _exit(error == ENOENT ? 127 : 126);
  }

  close(pipefd[0]);
  close(pipefd[1]);

  if (waitpid(left_pid, NULL, 0) == -1) {
    perror("Cron: waitpid");
  }

  if (waitpid(right_pid, NULL, 0) == -1) {
    perror("Cron: waitpid");
  }
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
