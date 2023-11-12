#include "menu.h"
#include "colors.h"
#include "main.h"
#include "term_size.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern TerminalSize term_size; // from src/main.c

void menu_item(char *string, char *color1, char *color2)
{
    size_t string_size = strlen(string);
    printf("\033[%d;%zdH%s%s%s%s", term_size.rows, term_size.columns - string_size,
           color1, color2, string, RESET);
}

void menu_centered_item(char *string, char *color1, char *color2,
                        int32_t row_offset)
{

    size_t string_size = strlen(string);
    printf("\033[%d;%zdH%s%s%s%s", (term_size.rows / 2) + row_offset,
           (term_size.columns - string_size) / 2, color1, color2, string, RESET);
}

void clear_menu(void) { printf("\033[2J"); }

void cursor_upLeft(void) { printf("\033[H"); }

void hide_cursor(void) { printf("\033[?25l"); }

void show_cursor(void) { printf("\033[?25h"); }

void menu_introduzir_artigo(void)
{

    Input inputItems[] = {
        {"Nome", ""},
        {"Preço", ""},
        {"Quantidade", ""},
    };
    Artigo artigo;
    int32_t result = input_menu(&artigo, inputItems, LENGTH(inputItems));
    printf("result: %d\n", result);
    printf("Nome: %s\n", inputItems[0].input);
}
void menu_listar(void)
{
    printf("Listar artigos\n");
}
void menu_modificar(void)
{
    printf("Modificar artigos\n");
}
void menu_estatisticas(void)
{
    char *options[] = {
        "Stock", // Alto e baixo preços | Quantidade maior e menor
        "Vendas",
        "Quebras", // Lixo | menu btop filtrar por categoria e total
        "Voltar"};

    int32_t totalOptions = (int)LENGTH(options);
    int32_t result = arrow_menu(options, totalOptions);
    printf("result: %d\n", result);
}
void menu_simular_vendas(void)
{
    printf("Simular vendas\n"); // menu nmtui | nome, codigo, preço, (quantidade) | guardar fatura num ficheiro txt
}

void menu_principal(void)
{

    char *options[] = {
        "Introduzir informação sobre um artigo",
        "Listar Artigos",
        "Modificar preço/stock",
        "Estatisticas",
        "Simular Venda",
        "Sair"};

    int32_t totalOptions = (int)LENGTH(options);
    int32_t result = arrow_menu(options, totalOptions);
    clear_menu();
    switch (result)
    {
    case 0:
        menu_introduzir_artigo();
        break;
    case 1:
        menu_listar();
        break;
    case 2:
        menu_modificar();
        break;
    case 3:
        menu_estatisticas();
        break;
    case 4:
        menu_simular_vendas();
        break;
    case 5:
        return;
        break;
    default:
        exit(1);
        break;
    }
}