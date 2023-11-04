#ifndef MENU_H
#define MENU_H

#include <stdint.h>

void menu_item(char *string, char *color1, char *color2);
void menu_centered_item(char *string, char *color1, char *color2, int32_t row_offset);
void clear_menu();

#endif // MENU_H