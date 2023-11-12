#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include "main.h"
#include <stdlib.h>

typedef struct
{
    char options[5][40];
} Checkbox;
typedef struct
{
    char label[40];
    char input[40];
    int isCheckbox;
    char checkBoxOptions[5][40];
    // Checkbox checkbox;

} Input;

void menu_item(char *string, char *color1, char *color2);
void menu_centered_item(char *string, char *color1, char *color2,
                        int32_t row_offset);
void clear_menu(void);
int32_t arrow_menu(char *strings[], int32_t size);
void cursor_upLeft(void);
void hide_cursor(void);
void show_cursor(void);
#ifdef __unix__
void enableRawMode(void);
void disableRawMode(void);
#endif

void menu_principal(void);
int32_t input_menu(Artigo *artigo, Input inputItems[], int32_t inputItemsSize);
void printMenuItem(Input item, int32_t isSelected, int32_t offset);
void printMenuCheckbox(char *string, int32_t isSelected, int32_t isCheckboxSelected, int32_t offset);

#endif // MENU_H
