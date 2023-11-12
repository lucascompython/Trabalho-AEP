#include "main.h"
#include "colors.h"
#include "menu.h"
#include "json.h"
#include "term_size.h"
#include "uuid.h"
#include <locale.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h> // Para a welcome screen _getch() ler todas as teclas
#include <time.h>  // Para o srand() para uuid_gen()
#endif

// TODO: IVA

#ifdef _WIN32
void restoreCursor(
    int sig) // Windows queixa-se que os paremetros não são os mesmos
{
    UNUSED(sig); // unused parameter, tirar warning do windows
    show_cursor();
}
#elif __unix__
void restoreCursor() { show_cursor(); }
#endif

TerminalSize term_size;
Artigo *artigos;
size_t size_artigos;
Artigo *artigos_vendidos;
size_t size_artigos_vendidos;

void welcome_screen(void)
{
    clear_menu();

    menu_centered_item("Bem vindo ao sistema de gestão de stocks", GREEN, BOLD,
                       0);
    menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "",
                       1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
    enableRawMode();
    getchar();
#elif _WIN32
    _getch(); // ler qualquer tecla no windows
#endif

    clear_menu();
    menu_principal();
}

int main(void)
{
    setlocale(LC_ALL, "Portuguese.UTF8");

#ifdef _WIN32
    srand((unsigned int)time(NULL)); // seed para o uuid_gen() no Windows
#endif
    hide_cursor();

    // restaure o cursor quando o programa terminar
    signal(SIGINT, restoreCursor);  // Ctrl + C
    signal(SIGTERM, restoreCursor); // kill
    signal(SIGSEGV, restoreCursor); // Segmentation fault
    atexit(show_cursor);            // Quando exit() é chamado | Não é 'restoreCursor' porque o Windows queixa-se que os paremetros não são os mesmos

    if (get_terminal_size(&term_size) != 0)
    {
        fprintf(stderr, "Erro a conseguir o tamanho do terminal...\n");
        return 1;
    }

    artigos = get_artigos_array(&size_artigos, STOCK_JSON_FILE);
    artigos_vendidos = get_artigos_array(&size_artigos_vendidos,
                                         VENDAS_JSON_FILE);

    welcome_screen();
    show_cursor(); // Restaurar o cursor ao sair. Não é 'restoreCursor' porque o Windows queixa-se que os paremetros não são os mesmos

    save_artigos_array(artigos, size_artigos, STOCK_JSON_FILE);
    save_artigos_array(artigos_vendidos, size_artigos_vendidos,
                       VENDAS_JSON_FILE);
    clean_artigos_array(artigos, size_artigos);
    clean_artigos_array(artigos_vendidos, size_artigos_vendidos);
    return 0;
}
