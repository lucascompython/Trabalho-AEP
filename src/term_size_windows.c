#include "term_size.h"

#ifdef _WIN32

#include <windows.h>

int get_terminal_size(TerminalSize *size)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
    {

        size->columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        size->rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        return 0; // Success
    }
    else
    {
        return -1; // Error
    }
}

#endif