#include "line_editor.h"
#include <stdio.h>

int main(void) {
  char *input = read_line("test> ");

  if (input == NULL) {
    printf("Input ended or  failed.\n");
    return 1;
  }

  printf("You types: %s\n", input);

  return 0;
}
