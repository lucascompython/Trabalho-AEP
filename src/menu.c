#include "menu.h"
#include "term_size.h"
#include "colors.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern TerminalSize term_size; // from src/main.c

void menu_item(char *string, char *color1, char *color2)
{
    int32_t string_size = strlen(string);
    printf("\033[%d;%dH%s%s%s%s", term_size.rows, term_size.columns - string_size, color1,
           color2, string, RESET);
}

void menu_centered_item(char *string, char *color1,
                        char *color2, int32_t row_offset)
{

    int32_t string_size = strlen(string);
    printf("\033[%d;%dH%s%s%s%s", (term_size.rows / 2) + row_offset, (term_size.columns - string_size) / 2,
           color1, color2, string, RESET);
}

void clear_menu()
{
    printf("\033[2J");
}