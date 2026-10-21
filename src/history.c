#include "history.h"

#include <stdio.h>
#include <string.h>

#define HISTORY_MAX 100
#define COMMAND_LENGTH 1024

static char commands[HISTORY_MAX][COMMAND_LENGTH];
static int command_count = 0;

void history_add(const char *command) {
  if (command_count >= HISTORY_MAX) {
    return;
  }

  strcpy(commands[command_count], command);
  command_count++;
}

void history_print(void) {
  for (int i = 0; i < command_count; i++) {
    printf("%d %s\n", i + 1, commands[i]);
  }
}

const char *history_get(int index) {
  if (index < 0 || index >= command_count) {
    return NULL;
  }

  return commands[index];
}

int history_get_count(void) { return command_count; }
