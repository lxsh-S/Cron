#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  char input[1024];

  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';

  // Find the pipe
  char *pipe_pos = strchr(input, '|');

  if (pipe_pos == NULL) {
    printf("No pipe found :(\n");
    return 0;
  }

  // Split the string at '|'
  *pipe_pos = '\0';

  char *left = input;
  char *right = pipe_pos + 1;

  // Parse left command
  char *left_args[64];
  char left_argc = 0;

  char *token = strtok(left, " ");

  while (token != NULL && left_argc < 63) {
    left_args[left_argc] = token;
    left_argc++;

    token = strtok(NULL, " ");
  }

  // Add 'NULL' at end!!
  left_args[left_argc] = NULL;

  // Parse the right command
  char *right_args[64];
  char right_argc = 0;

  token = strtok(right, " ");

  while (token != NULL && right_argc < 63) {
    right_args[right_argc] = token;
    right_argc++;

    token = strtok(NULL, " ");
  }

  // Add 'NULL' at the end!
  right_args[right_argc] = NULL;

  // Print what we parsed
  printf("LEFT:\n");

  for (int i = 0; left_args[i] != NULL; i++) {
    printf("[%d] = %s\n", i, left_args[i]);
  }

  printf("RIGHT:\n");

  for (int i = 0; right_args[i] != NULL; i++) {
    printf("[%d] = %s\n", i, right_args[i]);
  }

  return 0;
}
