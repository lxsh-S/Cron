#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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

static void redraw_line(const char *prompt, const char *buffer) {
  printf("\r\033[K%s%s", prompt, buffer);
  fflush(stdout);
}

static int is_command_position(const char *buffer, size_t word_start) {
  for (size_t i = 0; i < word_start; i++) {
    if (!isspace((unsigned char)buffer[i])) {
      return 0;
    }
  }

  return 1;
}

static int is_executable(const char *directory, const char *name) {
  char path[PATH_SIZE];
  // struct stat info;

  int written = snprintf(path, sizeof(path), "%s/%s", directory, name);

  if (written < 0 || (size_t)written >= sizeof(path)) {
    return 0;
  }

  return access(path, X_OK) == 0;
}

static void add_match(char matches[MAX_MATCHES][TOKEN_SIZE],
                      size_t *match_count, int *too_many, const char *name) {
  for (size_t i = 0; i < *match_count; i++) {
    if (strcmp(matches[i], name) == 0) {
      return;
    }
  }

  if (*match_count >= MAX_MATCHES) {
    *too_many = 1;
    return;
  }

  if (strlen(name) >= TOKEN_SIZE) {
    return;
  }

  strcpy(matches[*match_count], name);
  (*match_count)++;
}

static void collect_path_matches(const char *prefix,
                                 char matches[MAX_MATCHES][TOKEN_SIZE],
                                 size_t *match_count, int *too_many) {
  const char *path = getenv("PATH");

  if (path == NULL) {
    return;
  }

  const char *part = path;

  while (1) {
    const char *colon = strchr(part, ':');
    size_t dir_length = colon ? (size_t)(colon - part) : strlen(part);

    char directory[PATH_SIZE];

    if (dir_length == 0) {
      strcpy(directory, ".");
    } else if (dir_length >= sizeof(directory)) {
      if (colon == NULL) {
        break;
      }

      part = colon + 1;
      continue;
    } else {
      memcpy(directory, part, dir_length);
      directory[dir_length] = '\0';
    }

    DIR *dir = opendir(directory);

    if (dir != NULL) {
      struct dirent *entry;

      while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && prefix[0] != '.') {
          continue;
        }

        if (!starts_with(entry->d_name, prefix)) {
          continue;
        }

        if (is_executable(directory, entry->d_name)) {
          add_match(matches, match_count, too_many, entry->d_name);
        }

        if (*too_many) {
          break;
        }
      }

      closedir(dir);
    }

    if (*too_many || colon == NULL) {
      break;
    }

    part = colon + 1;
  }
}

size_t completion_handle(char *buffer, size_t length, size_t capacity,
                         const char *prompt) {
  if (buffer == NULL || prompt == NULL || capacity == 0 || length >= capacity) {
    return length;
  }

  // les find beginning of last word
  size_t word_start = length;

  while (word_start > 0 && !isspace((unsigned char)buffer[word_start - 1])) {
    word_start--;
  }

  char token[TOKEN_SIZE];
  size_t token_length = length - word_start;

  if (token_length >= sizeof(token)) {
    return length;
  }

  memcpy(token, buffer + word_start, token_length);
  token[token_length] = '\0';

  // we  seperate the directory path from the fimename's prefix
  char directory[PATH_SIZE] = ".";
  char path_prefix[TOKEN_SIZE] = "";
  char *name_prefix = token;

  char *slash = strrchr(token, '/');

  if (slash != NULL) {
    size_t directory_length = (size_t)(slash - token);
    size_t prefix_length = directory_length + 1;

    if (prefix_length >= sizeof(path_prefix) ||
        directory_length >= sizeof(directory)) {
      return length;
    }

    memcpy(path_prefix, token, prefix_length);
    path_prefix[prefix_length] = '\0';

    name_prefix = slash + 1;

    if (directory_length == 0) {
      strcpy(directory, "/");
    } else {
      memcpy(directory, token, directory_length);
      directory[directory_length] = '\0';
    }
  }

  char matches[MAX_MATCHES][TOKEN_SIZE];
  size_t match_count = 0;
  int too_many = 0;

  int completing_command =
      slash == NULL && is_command_position(buffer, word_start);

  if (completing_command) {
    collect_path_matches(name_prefix, matches, &match_count, &too_many);
  } else {
    DIR *dir = opendir(directory);

    if (dir == NULL) {
      return length;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] == '.' && name_prefix[0] != '.') {
        continue;
      }

      size_t prefix_length = strlen(path_prefix);
      size_t name_length = strlen(entry->d_name);

      if (prefix_length + name_length >= TOKEN_SIZE) {
        continue;
      }

      if (match_count == MAX_MATCHES) {
        too_many = 1;
        break;
      }

      memcpy(matches[match_count], path_prefix, prefix_length);
      memcpy(matches[match_count] + prefix_length, entry->d_name,
             name_length + 1);

      match_count++;
    }

    closedir(dir);
  }

  if (match_count == 0) {
    return length;
  }

  // if we find one match, we complete it and add a slash or a space
  if (match_count == 1) {
    size_t completed_length = strlen(matches[0]);
    char suffix = (!completing_command && is_directory(matches[0])) ? '/' : ' ';

    if (completed_length + 1 >= capacity - word_start) {
      return length;
    }

    memcpy(buffer + word_start, matches[0], completed_length);
    buffer[word_start + completed_length] = suffix;
    buffer[word_start + completed_length + 1] = '\0';
    return word_start + completed_length + 1;
  }

  // and if we get multiple matches, we find the longest common prefix
  if (!too_many) {
    size_t common_length = strlen(matches[0]);

    for (size_t i = 1; i < match_count; i++) {
      while (common_length > 0 &&
             strncmp(matches[0], matches[i], common_length) != 0) {
        common_length--;
      }
    }

    if (common_length > token_length && common_length < capacity - word_start) {
      memcpy(buffer + word_start, matches[0], common_length);
      buffer[word_start + common_length] = '\0';
      return word_start + common_length;
    }
  }
  // oh yea what if we dont find a proper completion?, we display the avaible
  // matches
  printf("\n");

  for (size_t i = 0; i < match_count; i++) {
    printf("%s%s ", matches[i], is_directory(matches[i]) ? "/" : "");
  }

  if (too_many) {
    printf("...");
  }

  printf("\n");
  redraw_line(prompt, buffer);

  return length;
}
