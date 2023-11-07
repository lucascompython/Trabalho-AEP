#ifndef MENU_H
#define MENU_H

#include <stdint.h>

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

#endif // MENU_H
