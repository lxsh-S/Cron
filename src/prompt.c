#include "prompt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char *prompt_get(void) {
  static char prompt[8192];
  char hostname[256] = "unknown";
  char cwd[4096] = "?";
  char display_path[4096];

  const char *username = getenv("USER");
  const char *home = getenv("HOME");

  if (username == NULL || username[0] == '\0') {
    username = "user";
  }

  if (gethostname(hostname, sizeof(hostname)) == -1) {
    strcpy(hostname, "unknown");
  }

  hostname[sizeof(hostname) - 1] = '\0';

  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    strcpy(cwd, "?");
  }

  if (home != NULL && home[0] != '\0') {
    size_t home_length = strlen(home);
    size_t cwd_length = strlen(cwd);

    if (home_length <= cwd_length && strncmp(cwd, home, home_length) == 0 &&
        (cwd[home_length] == '\0' || cwd[home_length] == '/')) {
      if (cwd[home_length] == '\0') {
        strcpy(display_path, "~");
      } else {
        snprintf(display_path, sizeof(display_path), "~%s", cwd + home_length);
      }
    } else {
      snprintf(display_path, sizeof(display_path), "%s", cwd);
    }
  } else {
    snprintf(display_path, sizeof(display_path), "%s", cwd);
  }

  snprintf(prompt, sizeof(prompt), "%s@%s:%s λ ", username, hostname,
           display_path);
  return prompt;
}
