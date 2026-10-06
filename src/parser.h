#ifndef PARSER_H
#define PARSER_H

void parse_command(char *input, char **args);

int split_pipe(char *input, char **left, char **right);

int split_redirect(char *input, char **command, char **file);

#endif // PARSER_H
