#ifndef EXECUTOR_H
#define EXECUTOR_H

void execute_command(char **args);
void execute_pipeline(char **left_args, char **right_args);
void execute_redirect(char **args, char *file, int append);
void execute_input_redirect(char **args, char *file);
#endif // EXECUTOR_H
