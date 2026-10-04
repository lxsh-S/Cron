#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char input[1024];

  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';

  // Find the pipe
  char *pipe_pos = strchr(input, '|');

  if (pipe_pos == NULL) {
    printf("No pipe found\n");
    return 1;
  }

  // Split input into two commands
  *pipe_pos = '\0';

  char *left = input;
  char *right = pipe_pos + 1;

  // Parse the left commad
  char *left_args[64];
  int left_argc = 0;

  char *token = strtok(left, " ");

  while (token != NULL && left_argc < 63) {
    left_args[left_argc] = token;
    left_argc++;

    token = strtok(NULL, " ");
  }

  left_args[left_argc] = NULL;

  // Parse right command
  char *right_args[64];
  int right_argc = 0;

  token = strtok(right, " ");

  while (token != NULL && right_argc < 63) {
    right_args[right_argc] = token;
    right_argc++;

    token = strtok(NULL, " ");
  }

  right_args[right_argc] = NULL;

  // Create pipe
  int pipefd[2];

  if (pipe(pipefd) == -1) {
    perror("pipe");
    return 1;
  }

  // First child

  pid_t left_pid = fork();

  if (left_pid == 0) {
    close(pipefd[0]);

    dup2(pipefd[1], STDOUT_FILENO);

    close(pipefd[1]);

    execvp(left_args[0], left_args);

    perror("exec");
    return 1;
  }

  // Second child
  pid_t right_pid = fork();

  if (right_pid == 0) {
    close(pipefd[1]);

    dup2(pipefd[0], STDIN_FILENO);

    close(pipefd[0]);

    execvp(right_args[0], right_args);

    perror("exec");

    return 1;
  }

  close(pipefd[0]);
  close(pipefd[1]);

  waitpid(left_pid, NULL, 0);
  waitpid(right_pid, NULL, 0);

  return 0;
}
