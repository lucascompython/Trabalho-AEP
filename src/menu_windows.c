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
    clear_menu();

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
                printf("%s\n", RESET); // Reset text color
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
            else if (key == ARROW_UP)
            {
                if (selectedOption > 0)
                    selectedOption--; // Up arrow key
                else
                    selectedOption = totalOptions - 1; // Voltar para o fim

                break;
            }
            else if (key == ARROW_DOWN)
            {
                if (selectedOption < totalOptions - 1)
                    selectedOption++; // Down arrow key
                else
                    selectedOption = 0; // Voltar para o inicio

                break;
            }
        }
    }
}

#endif