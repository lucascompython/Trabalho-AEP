#ifdef _WIN32
#include <conio.h>
#include <stdio.h>
#include "menu.h"
#include <stdint.h>
#include <windows.h>

#define ARROW_UP 72
#define ARROW_DOWN 80
#define ENTER 13

int arrow_menu(char *strings[], int size)
{

    int selectedOption = 0;
    int totalOptions = size;

    while (1)
    {
        // Print menu options
        for (int i = 0; i < totalOptions; i++)
        {
            if (i == selectedOption)
            {
                printf("\033[7m"); // Invert text color (selected option)
                menu_centered_item(strings[i], "", "", i);
                printf("\033[0m\n"); // Reset text color
            }
            else
            {
                menu_centered_item(strings[i], "", "", i);
                puts("");
            }
        }

        // Handle arrow key input
        char key = 0;
        while (1)
        {
            key = getch();
            if (key == ENTER) // Enter key
            {
                return selectedOption;
            }
            else if (key == ARROW_UP && selectedOption > 0)
            {
                selectedOption--; // Up arrow key
                break;
            }
            else if (key == ARROW_DOWN && selectedOption < totalOptions - 1)
            {
                selectedOption++; // Down arrow key
                break;
            }
        }
    }
}

#endif