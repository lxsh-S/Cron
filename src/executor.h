#ifndef EXECUTOR_H
#define EXECUTOR_H

void execute_command(char **args);
void execute_pipeline(char **left_args, char **right_args);

#endif // EXECUTOR_H
