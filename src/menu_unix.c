#ifdef __unix__
#include <unistd.h>
#include <termios.h>
#include "menu.h"
#include <stdint.h>
#include <stdio.h>
#include "colors.h"
#include "main.h"
#include "term_size.h"
#include <string.h>
#include <stdlib.h>

#define ESC 27

extern TerminalSize term_size; // from src/main.c

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

int32_t arrow_menu(char *strings[], int32_t size)
{
    clear_menu();
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
                menu_centered_item(strings[i], "", "", i - (totalOptions / 2) + 1);
                printf("%s\n", RESET); // Reset text color
            }
            else
            {

                menu_centered_item(strings[i], "", "", i - (totalOptions / 2) + 1);
                puts("");
            }
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
                if (c == 'A')
                {
                    if (selectedOption > 0)
                        selectedOption--; // Up arrow key
                    else
                        selectedOption = totalOptions - 1; // Voltar ao fim
                    break;
                }
                else if (c == 'B')
                {
                    if (selectedOption < totalOptions - 1)
                        selectedOption++; // Down arrow key
                    else
                        selectedOption = 0; // Voltar ao inicio
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

int32_t input_menu(Artigo *artigo, Input inputItems[], int32_t inputItemsSize)
{
    clear_menu();
    enableRawMode();
    UNUSED(artigo);

    const unsigned int maxInputSize = 40;

    int selectedButton = 1;
    int selectedItem = 0;
    int selectedCheckbox = -1; // -1 = Nenhum selecionado

    char c;

    while (1)
    {
        for (int i = 0; i < inputItemsSize; i++)
        {

            if (inputItems[i].isCheckbox)
            {
                if (i == selectedItem)
                {
                    printMenuItem(inputItems[i], 1, (i + 1) - ((inputItemsSize + 5) / 2));
                }
                else
                {

                    printMenuItem(inputItems[i], 0, (i + 1) - ((inputItemsSize + 5) / 2));
                }

                for (int j = 0; j < 5; j++)
                {

                    if (j == selectedCheckbox)
                    {
                        if (selectedItem > 3 && selectedItem == j + 4)
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 1, 1, i - (inputItemsSize / 2) + j);
                        }
                        else
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 0, 1, i - (inputItemsSize / 2) + j);
                        }
                    }
                    else
                    {
                        if (selectedItem > 3 && selectedItem == j + 4)
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 1, 0, i - (inputItemsSize / 2) + j);
                        }
                        else
                        {

                            printMenuCheckbox(inputItems[i].checkBoxOptions[j], 0, 0, i - (inputItemsSize / 2) + j);
                        }
                    }
                }
                continue;
            }

            if (i == selectedItem)
            {
                printMenuItem(inputItems[i], 1, i - ((inputItemsSize + 5) / 2) + 1);
            }
            else
            {
                printMenuItem(inputItems[i], 0, i - ((inputItemsSize + 5) / 2) + 1);
            }
        }

        if (selectedButton == 1)
        {
            printf("\033[%d;%dH\033[7m[OK]\033[0m   [Cancelar]\n", (term_size.rows / 2) + inputItemsSize + 2,
                   (term_size.columns - 17) / 2); // 17 é o tamanho da string "[OK]   [Cancel]"
        }
        else
        {
            printf("\033[%d;%dH[OK]   \033[7m[Cancelar]\033[0m\n", (term_size.rows / 2) + inputItemsSize + 2,
                   (term_size.columns - 17) / 2); // 17 é o tamanho da string "[OK]   [Cancel]"
        }

        c = getchar();
        if (c == ESC)
        {
            c = getchar(); // Read the [
            c = getchar(); // Read the A, B, C, or D
            if (c == 'A')  // Up Arrow
            {
                if (selectedItem > 0)
                    selectedItem--;
                else
                    selectedItem = inputItemsSize + 5 - 1; // Voltar ao fim
            }
            else if (c == 'B') // Down Arrow
            {
                if (selectedItem < inputItemsSize + 5 - 1)
                    selectedItem++;
                else
                    selectedItem = 0; // Voltar ao inicio
            }
            else if (c == 'D') // Left Arrow
            {
                if (selectedButton == 2)
                    selectedButton = 1;
                else
                    selectedButton = 2;
            }
            else if (c == 'C') // Right Arrow
            {
                if (selectedButton == 1)
                    selectedButton = 2;
                else
                    selectedButton = 1;
            }
        }
        else if (c == '\n')
        { // Enter Key
            if (selectedButton == 1)
            {
                disableRawMode();
                return 1;
            }
            else
            {
                disableRawMode();
                return 0;
            }
        }
        else if (c == 127)
        { // Backspace
            // if (selectedButton == 1)
            // {
            //     if (strlen(inputItems[selectedItem].input) > 0)
            //     {
            //         inputItems[selectedItem].input[strlen(inputItems[selectedItem].input) - 1] = '\0';
            //     }
            // }
            if (selectedItem >= 0 && selectedItem <= inputItemsSize - 1)
            {
                // Remove the last character from the input
                size_t len = strlen(inputItems[selectedItem].input);
                if (len > 0)
                {
                    inputItems[selectedItem].input[len - 1] = '\0';
                }
            }
        }
        else if (c == 32) // Barra de espaço
        {
            if (selectedItem > 3)
            {
                selectedCheckbox = selectedItem - 4;
            }
        }

        else
        { // Handle input characters
            if (selectedItem >= 0 && selectedItem <= inputItemsSize - 1 && !inputItems[selectedItem].isCheckbox)
            {
                if (inputItems[selectedItem].input[strlen(inputItems[selectedItem].input) - 1] == '*')
                {
                    inputItems[selectedItem].input[strlen(inputItems[selectedItem].input) - 1] = c;
                }
                else
                {
                    if (strlen(inputItems[selectedItem].input) < maxInputSize - 1)
                    {
                        inputItems[selectedItem].input[strlen(inputItems[selectedItem].input)] = c;
                        // inputItems[selectedItem].input[strlen(inputItems[selectedItem].input)] = '*';
                        // strncat(inputItems[selectedItem].input, &c, 1);
                    }
                }
            }
        }
        // else if (c >= 32 && c <= 126)
        // { // Printable characters
        //     if (selectedButton == 1)
        //     {
        //         if (strlen(inputItems[selectedItem].input) < maxInputSize)
        //         {
        //             inputItems[selectedItem].input[strlen(inputItems[selectedItem].input)] = c;
        //         }
        //     }
        // }
        cursor_upLeft();
        clear_menu();
    }

    return 0;
}

#elif _WIN32

#pragma warning(disable : 4206) // Tirar aviso de "nonstandard extension used: translation unit is empty" porque este ficheiro só é usado no linux

#endif