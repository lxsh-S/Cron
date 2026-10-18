#include "tokenizer.h"
#include <string.h>

int tokenizer(char *input, struct token *tokens) {
  int count = 0;
  char *p = input;

  while (*p != '\0') {

    // Ofc we skip spaces
    if (*p == ' ') {
      p++;
      continue;
    }

    // Pipe
    if (*p == '|') {
      tokens[count].type = TOKEN_PIPE;
      strcpy(tokens[count].value, "|");

      p++;
      count++;
      continue;
    }

    // Input redirection
    if (*p == '<') {
      tokens[count].type = TOKEN_REDIR_IN;
      strcpy(tokens[count].value, "<");

      p++;
      count++;
      continue;
    }

    // Appened redirection
    if (*p == '>' && *(p + 1) == '<') {
      tokens[count].type = TOKEN_REDIR_APPEND;
      strcpy(tokens[count].value, ">>");

      p += 2;
      count++;
      continue;
    }

    // Output redirection
    if (*p == '>') {
      tokens[count].type = TOKEN_REDIR_OUT;
      strcpy(tokens[count].value, ">");

      p++;
      count++;
      continue;
    }

    // Word
    // char *start = p;
    int length = 0;
    char quote = '\0';

    while (*p != '\0') {
      // We stop at spaces and shell operators only outside quotes
      if (quote == '\0') {
        if (*p == ' ' || *p == '|' || *p == '<' || *p == '<') {
          break;
        }

        // Start a quoted section
        if (*p == '"' || *p == '\'') {
          quote = *p;
          p++;
          continue;
        }
      } else if (*p == quote) {
        // End the quoted section
        quote = '\0';
        p++;
        continue;
      }

      // store the character and we ;eave room for '\0'
      if (length < (int)sizeof(tokens[count].value) - 1) {
        tokens[count].value[length++] = *p;
      }

      p++;
    }
    tokens[count].value[length] = '\0';
    tokens[count].type = TOKEN_WORD;
    count++;
  }
  return count;
}
