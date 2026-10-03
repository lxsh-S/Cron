#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {

  char input[1024]; // Initializing the user input variable

  while (1) {
    printf(">>> ");
    fgets(input, sizeof(input), stdin); // Read user input
    input[strcspn(input, "\n")] = '\0'; // remove the trailing lines

    // Comapre the user input with 'exit'
    if (strcmp(input, "exit") == 0) {
      break;
    }

    if (input[0] != '\0') {
      char *args[64];
      int argc = 0;

      char *token = strtok(input, " ");

      while (token != NULL && argc <= 63) {
        args[argc] = token;
        argc++;
        token = strtok(NULL, " ");
      }

      args[argc] = NULL;

      pid_t pid = fork();

      if (pid == 0) {
        execvp(args[0], args);

        printf("%s: command not found\n", args[0]);
        exit(1);
      } else {
        wait(NULL);
      }
    }
  }
  return 0;
}
