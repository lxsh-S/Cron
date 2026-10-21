#ifndef HISTORY_H
#define HISTORY_H

void history_add(const char *command);
void history_print(void);
const char *history_get(int index); // we will return command at one index
int history_get_count(
    void); // also we will need how many commands we have stored

void history_init();
#endif // !HISTORY_H
