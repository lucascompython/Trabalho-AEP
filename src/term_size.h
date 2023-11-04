#ifndef TERM_SIZE_H
#define TERM_SIZE_H

#ifdef __unix__

#include <sys/ioctl.h>
#include <unistd.h>

#elif defined(_WIN32) || defined(WIN32)

#include <windows.h>

#endif

typedef struct
{
    int rows;
    int columns;
} TerminalSize;

int get_terminal_size(TerminalSize *size);

#endif // TERM_SIZE_H