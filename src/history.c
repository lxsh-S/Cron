#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HISTORY_MAX 100
#define COMMAND_LENGTH 1024

static char commands[HISTORY_MAX][COMMAND_LENGTH];
static int command_count = 0;
static char history_path[2048] = "";

// store a command in the memory hitory
static void store_command(const char *command) {
  if (command_count < HISTORY_MAX) {
    strcpy(commands[command_count], command);
    command_count++;
    return;
  }

  // history is full: discard the oldest command
  for (int i = 1; i < HISTORY_MAX; i++) {
    strcpy(commands[i - 1], commands[i]);
  }

  strcpy(commands[HISTORY_MAX - 1], command);
}

// load saved commands when we restart cron
void history_init(void) {
  const char *home = getenv("HOME");

  if (home == NULL) {
    return;
  }

  int length =
      snprintf(history_path, sizeof(history_path), "%s/.cron_history", home);

  if (length < 0 || (size_t)length >= sizeof(history_path)) {
    history_path[0] = '\0';
    return;
  }

  FILE *file = fopen(history_path, "r");

  if (file == NULL) {
    return;
  }

  char line[COMMAND_LENGTH];

  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (line[0] != '\0') {
      store_command(line);
    }
  }

  fclose(file);
}

// add a command to memory and then we save it to disk
void history_add(const char *command) {
  if (command == NULL || command[0] == '\0') {
    return;
  }

  store_command(command);

  if (history_path[0] == '\0') {
    return;
  }

  FILE *file = fopen(history_path, "a");

  if (file == NULL) {
    return;
  }

  fprintf(file, "%s\n", command);
  fclose(file);
}

void history_print(void) {
  for (int i = 0; i < command_count; i++) {
    printf("%d %s\n", i + 1, commands[i]);
  }
}

const char *history_get(int index) {
  if (index < 0 || index >= command_count) {
    return NULL; // beacause invalid
  }

  return commands[index];
}

int history_get_count(void) { return command_count; }
