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

    welcome_screen();
    show_cursor(); // Restaurar o cursor ao sair. Não é 'restoreCursor' porque o Windows queixa-se que os paremetros não são os mesmos

    char *uuid = uuid_gen();
    printf("UUID: %s\n", uuid);

    size_t size_artigos;
    Artigo *artigos = get_artigos_array(&size_artigos);
    puts("----------------------");
    for (size_t i = 0; i < size_artigos; i++)
    {
        printf("NOME: %s\n", artigos[i].nome);
        printf("CATEGORIA: %d\n", artigos[i].categoria);
        printf("PRECO: %f\n", artigos[i].preco);
        printf("QUANTIDADE: %ld\n", artigos[i].quantidade);
        printf("UUID: %s\n", artigos[i].uuid);
        puts("----------------------");
    }
    clean_artigos_array(artigos, size_artigos);
    return 0;
}
