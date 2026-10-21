#include "history.h"
#include "line_editor.h"

#include <stdio.h>

int main(void) {
  history_add("echo hello");
  history_add("pwd");
  history_add("ls -la");

  char *input = read_line("test> ");

  if (input == NULL) {
    printf("Input ended or failed.\n");
    return 1;
  }

  printf("You typed this shit!: %s\n", input);

  return 0;
}
