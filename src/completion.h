#ifndef COMPLETION_H
#define COMPLETION_H

#include <stddef.h>

size_t completion_handle(char *buffer, size_t length, size_t capacity,
                         const char *prompt);

#endif
