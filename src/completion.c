#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "completion.h"

#define PATH_SIZE 4096
#define TOKEN_SIZE 1024
#define MAX_MATCHES 64

static int starts_with(const char *text, const char *prefix) {
  return strncmp(text, prefix, strlen(prefix)) == 0;
}

static int is_directory(const char *path) {
  struct stat info;

  if (stat(path, &info) == -1) {
    return 0;
  }

  return S_ISDIR(info.st_mode);
}
