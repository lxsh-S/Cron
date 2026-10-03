#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {

  char input[1024]; // Initializing the user input variable

  while (1) {
    printf(">>> ");
    fgets(input, sizeof(input), stdin); // Read user input
    input[strcspn(input, "\n")] = '\0'; // remove the trailing lines

    // Comapre the user input with 'exit'
    if (strcmp(input, "exit") == 0) {
      break;

    } else if (input[0] != '\0') { // We run the command and print the input
                                   // back using "system"
      int result = system(input);

      if (result != 0) { // If the command is not found we return a error.
        printf("%s: not found\n", input);
        break;
      }
    }
  }
  return 0;
}
