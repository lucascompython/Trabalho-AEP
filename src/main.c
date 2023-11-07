#include "main.h"
#include "colors.h"
#include "menu.h"
#include "term_size.h"
#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void restoreCursor()
{
    show_cursor();
}

TerminalSize term_size;

void welcome_screen(void)
{
    clear_menu();

    menu_centered_item("Bem vindo ao sistema de gestão de stocks", GREEN, BOLD,
                       0);
    menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "",
                       1);
    puts("");

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
    enableRawMode();
#endif

    getchar();

    clear_menu();
    menu_principal();
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese.UTF8");

    hide_cursor();
    // restaure o cursor quando o programa terminar
    signal(SIGINT, restoreCursor);  // Ctrl + C
    signal(SIGTERM, restoreCursor); // kill
    signal(SIGSEGV, restoreCursor); // Segmentation fault
    atexit(restoreCursor);          // Quando exit() é chamado

    if (get_terminal_size(&term_size) != 0)
    {
        fprintf(stderr, "Erro a conseguir o tamanho do terminal...\n");
        return 1;
    }

    welcome_screen();
    restoreCursor(); // Restore cursor on exit
    return 0;
}
