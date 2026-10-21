#include "line_editor.h"

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static int enable_raw_mode(struct termios *original) {
  struct termios raw;

  if (tcgetattr(STDIN_FILENO, original) == -1) {
    return -1;
  }

  raw = *original;
  raw.c_lflag &= -(ICANON | ECHO);

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    return -1;
  }
  return 0;
}

static void disable_raw_mode(const struct termios *original) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, original);
}

char *read_line(const char *prompt) {
  static char buffer[1024];
  size_t length = 0;
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

    if (c == '\n' || c == '\r') {
      break;
    }

    if (c == 127 || c == '\b') {
      if (length > 0) {
        length++;
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
