#ifndef PARSER_H
#define PARSER_H

// as we are using the struct token
#include "tokenizer.h"

void parse_command(char *input, char **args);

int split_pipe(char *input, char **left, char **right);

int split_redirect(char *input, char **command, char **file);

int split_input_redirect(char *input, char **command, char **file);

int split_tokens_pipe(struct token *tokens, int count, char **left_args,
                      char **right_args);

#endif // PARSER_H
