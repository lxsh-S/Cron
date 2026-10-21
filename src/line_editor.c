#include "line_editor.h"
#include "completion.h"
#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static int enable_raw_mode(struct termios *original) {
  struct termios raw;

  if (tcgetattr(STDIN_FILENO, original) == -1) {
    return -1;
  }

  raw = *original;
  raw.c_lflag &= ~(ICANON | ECHO | ISIG);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    return -1;
  }
  return 0;
}

static void disable_raw_mode(const struct termios *original) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, original);
}

static int read_escape_sequence(void) {
  char sequence[2];

  if (read(STDIN_FILENO, &sequence[0], 1) != 1) {
    return 0;
  }

  if (sequence[0] != '[') {
    return 0;
  }

  if (read(STDIN_FILENO, &sequence[1], 1) != 1) {
    return 0;
  }

  switch (sequence[1]) {
  case 'A':
    return 1; // Up
  case 'B':
    return 2; // Down
  default:
    return 0;
  }
}

char *read_line(const char *prompt) {
  static char buffer[1024];
  size_t length = 0;
  int history_position = history_get_count();
  struct termios original;

  printf("%s", prompt);
  fflush(stdout);

  if (enable_raw_mode(&original) == -1) {
    return NULL;
  }

  while (1) {
    char c;
    ssize_t bytes_read = read(STDIN_FILENO, &c, 1);

    if (bytes_read <= 0) {
      disable_raw_mode(&original);
      return NULL;
    }

    // if ctrl c we cancel the curent input
    if (c == 3) { // BYTE VALUE FOR CTRL C
      buffer[0] = '\0';
      disable_raw_mode(&original);
      printf("^C\n");
      return buffer;
    }

    // ctrl o end input i line is empty
    if (c == 4) { // byte value for ctrl d
      if (length == 0) {
        disable_raw_mode(&original);
        return NULL;
      }

      break;
    }

    if (c == '\n' || c == '\r') {
      break;
    }

    if (c == '\t') {
      length = completion_handle(buffer, length, sizeof(buffer), prompt);

      printf("\r\033[K%s%s", prompt, buffer);
      fflush(stdout);

      continue;
    }

    if (c == 27) {
      int key = read_escape_sequence();
      const char *command = NULL;

      if (key == 1 && history_position > 0) {
        history_position--;
        command = history_get(history_position);
      } else if (key == 2 && history_position < history_get_count()) {
        history_position++;

        if (history_position < history_get_count()) {
          command = history_get(history_position);
        } else {
          command = "";
        }
      }

      if (command != NULL) {
        printf("\r\033[K%s", prompt);

        strcpy(buffer, command);
        length = strlen(buffer);

        printf("%s", buffer);
        fflush(stdout);
      }

      continue;
    }

    if (c == 127 || c == '\b') {
      if (length > 0) {
        length--;
        printf("\b \b");
        fflush(stdout);
      }
      continue;
    }

    if (length < sizeof(buffer) - 1 && c >= 32 && c <= 126) {
      buffer[length++] = c;
      putchar(c);
      fflush(stdout);
    }
  }

  buffer[length] = '\0';
  disable_raw_mode(&original);
  putchar('\n');

  return buffer;
}
