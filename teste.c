#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define MENU_ITEMS 7
#define MAX_LABEL_LENGTH 40
#define MAX_INPUT_LENGTH 40

typedef struct MenuItem
{
    char label[MAX_LABEL_LENGTH];
    char input[MAX_INPUT_LENGTH];
} MenuItem;

void clearScreen()
{
    printf("\033[H\033[J");
}

void printMenuItem(MenuItem item, int isInput, int isSelected)
{
    if (isInput)
    {
        if (isSelected)
        {
            printf(" > %s: %s\n", item.label, item.input);
        }
        else
        {
            printf("   %s: %s\n", item.label, item.input);
        }
    }
    else
    {
        if (isSelected)
        {
            printf(" > \033[7m%s\033[0m\n", item.label);
        }
        else
        {
            printf("   %s\n", item.label);
        }
    }
}

int main()
{
    setbuf(stdout, NULL); // Disable output buffering
    struct termios originalTermios;
    tcgetattr(STDIN_FILENO, &originalTermios);

    struct termios newTermios = originalTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTermios);

    char arrow;
    int selectedItem = 0;
    int isRunning = 1;
    int selectedButton = 1; // 1 for "OK", 2 for "Cancel"

    MenuItem menuItems[MENU_ITEMS] = {
        {"Profile name", "nosso"},
        {"Device", "wlp13s0 (10:6F:D9:DD:15:A5)"},
        {"WI-FI", ""},
        {"SSID", "nosso"},
        {"Mode", "<Client>"},
        {"Security", "<WPA & WPA2 Personal>"},
        {"Password", "********"},
    };

    while (isRunning)
    {
        clearScreen();

        for (int i = 0; i < MENU_ITEMS; i++)
        {
            if (i == selectedItem)
            {
                if (i >= 0 && i <= 6)
                {
                    printMenuItem(menuItems[i], 1, 1);
                }
                else
                {
                    printMenuItem(menuItems[i], 0, 1);
                }
            }
            else
            {
                if (i >= 0 && i <= 6)
                {
                    printMenuItem(menuItems[i], 1, 0);
                }
                else
                {
                    printMenuItem(menuItems[i], 0, 0);
                }
            }
        }

        // Highlight "OK" or "Cancel" button
        if (selectedButton == 1)
        {
            printf(" > \033[7m[OK]\033[0m [Cancel]\n\n");
        }
        else
        {
            printf(" > [OK] \033[7m[Cancel]\033[0m\n\n");
        }

        arrow = getchar();
        if (arrow == 27)
        {
            char next = getchar();
            if (next == 91)
            {
                char key = getchar();
                if (key == 65)
                {
                    // Up arrow
                    if (selectedItem > 0)
                    {
                        selectedItem--;
                    }
                }
                else if (key == 66)
                {
                    // Down arrow
                    if (selectedItem < MENU_ITEMS - 1)
                    {
                        selectedItem++;
                    }
                }
                else if (key == 68)
                {
                    // Left arrow
                    if (selectedButton == 2)
                    {
                        selectedButton = 1;
                    }
                }
                else if (key == 67)
                {
                    // Right arrow
                    if (selectedButton == 1)
                    {
                        selectedButton = 2;
                    }
                }
            }
        }
        else if (arrow == 10)
        {
            // Enter key
            isRunning = 0;
        }
        else if (arrow == 127)
        {
            // Backspace (delete) key
            if (selectedItem >= 0 && selectedItem <= 6)
            {
                // Remove the last character from the input
                size_t len = strlen(menuItems[selectedItem].input);
                if (len > 0)
                {
                    menuItems[selectedItem].input[len - 1] = '\0';
                }
            }
        }
        else
        {
            // Handle input characters
            if (selectedItem >= 0 && selectedItem <= 6)
            {
                if (menuItems[selectedItem].input[strlen(menuItems[selectedItem].input) - 1] == '*')
                {
                    menuItems[selectedItem].input[strlen(menuItems[selectedItem].input) - 1] = arrow;
                }
                else
                {
                    if (strlen(menuItems[selectedItem].input) < MAX_INPUT_LENGTH - 1)
                    {
                        strncat(menuItems[selectedItem].input, &arrow, 1);
                    }
                }
            }
        }
    }

    clearScreen();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios);

    // Print the final menu state
    for (int i = 0; i < MENU_ITEMS; i++)
    {
        if (i >= 0 && i <= 6)
        {
            printMenuItem(menuItems[i], 1, 0);
        }
        else
        {
            printMenuItem(menuItems[i], 0, 0);
        }
    }

    // Print selected information and button
    printf("\nSelected Information: %s\nSelected Button: %s\n", menuItems[selectedItem].input, (selectedButton == 1) ? "OK" : "Cancel");

    return 0;
}
