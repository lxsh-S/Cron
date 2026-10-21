#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

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

  DIR *dir = opendir(directory);

  if (dir == NULL) {
    return length;
  }

  char matches[MAX_MATCHES][TOKEN_SIZE];
  size_t match_count = 0;
  int too_many = 0;
  struct dirent *entry;

  while ((entry = readdir(dir)) != NULL) {
    // we hide dotfiles unless the user types a dot
    if (entry->d_name[0] == '.' && name_prefix[0] != '.') {
      continue;
    }

    if (!starts_with(entry->d_name, name_prefix)) {
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

  if (match_count == 0) {
    return length;
  }

  // if we find one match, we complete it and add a slash or a space
  if (match_count == 1) {
    size_t completed_length = strlen(matches[0]);
    char suffix = is_directory(matches[0]) ? '/' : ' ';

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
}
