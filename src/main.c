#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void) {

  char input[1024];

  while (1) {
    printf(">>> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    if (input[0] != '\0') {
      int result = system(input);

      if (result != 0) {
        printf("%s: not found\n", input);
        break;
      }
    }
  }
  return 0;
}
