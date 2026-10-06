#ifndef PARSER_H
#define PARSER_H

void parse_command(char *input, char **args);

int split_pipe(char *input, char **left, char **right);

#endif // PARSER_H
