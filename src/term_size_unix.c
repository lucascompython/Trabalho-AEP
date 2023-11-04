#include "term_size.h"

#ifdef __unix__
#include <sys/ioctl.h>
#include <unistd.h>

int get_terminal_size(TerminalSize *size)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0)
    {
        size->rows = ws.ws_row;
        size->columns = ws.ws_col;
        return 0;
    }
    else
    {
        return -1;
    }
}

#endif