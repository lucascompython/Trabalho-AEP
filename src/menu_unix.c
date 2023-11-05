#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#include "menu.h"
#include "stdint.h"
#include <stdio.h>

#define ESC 27

// Function to enable raw mode for terminal input
void enableRawMode(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

// Function to disable raw mode for terminal input
void disableRawMode(void)
{
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
}

// function that creates a menu controlled by the arrow keys and returns the selected option
// accepts an array of strings with the options and the number of options
int32_t arrow_menu(char *strings[], int32_t size)
{
    enableRawMode();

    int32_t selectedOption = 0;
    int32_t totalOptions = size;

    while (1)
    {
        // Print menu options
        for (int32_t i = 0; i < totalOptions; i++)
        {
            if (i == selectedOption)
            {
                printf("\033[7m"); // Invert text color (selected option)
            }
            // printf("%s", strings[i]);
            menu_centered_item(strings[i], "", "", i);
            printf("\033[0m\n"); // Reset text color
        }

        // Handle arrow key input
        char c = 0;
        while (1)
        {
            c = getchar();
            if (c == ESC)
            {
                c = getchar(); // Read the [
                c = getchar(); // Read the A, B, C, or D
                if (c == 'A' && selectedOption > 0)
                {
                    selectedOption--; // Up arrow key
                    break;
                }
                else if (c == 'B' && selectedOption < totalOptions - 1)
                {
                    selectedOption++; // Down arrow key
                    break;
                }
            }
            else if (c == '\n')
            {
                disableRawMode();
                return selectedOption;
            }
        }
        clear_menu();
        cursor_upLeft();
    }
}

#endif