#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main() {
  char input[1024];
  char *args[64];
  int argc = 0;

  fgets(input, sizeof(input), stdin);
  input[strcspn(input, "\n")] = '\0';

  // Parse
  char *token = strtok(input, " ");

  while (token != NULL && argc < 63) {
    args[argc] = token;
    argc++;
    token = strtok(NULL, " ");
  }

  args[argc] = NULL;

  for (int i = 0; args[i] != NULL; i++) {
    printf("args[%d] = %s\n", i, args[i]);
  }

  return 0;
}
