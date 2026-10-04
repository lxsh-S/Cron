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

  printf("LEFT: %s\n", left);
  printf("RIGHT: %s\n", right);
}
