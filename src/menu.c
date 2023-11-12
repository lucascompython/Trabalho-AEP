#include "menu.h"
#include "colors.h"
#include "main.h"
#include "term_size.h"
#include "json.h"
#include "uuid.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern TerminalSize term_size; // from src/main.c
extern Artigo *artigos;        // from src/main.c
extern size_t size_artigos;    // from src/main.c

void printMenuItem(Input item, int32_t isSelected, int32_t offset)
{
    int labelLength = strlen(item.label);
    int inputLength = strlen(item.input);
    if (isSelected)
    {
        if (item.isCheckbox)
        {
            printf("\033[%d;%dH> %s%s%s <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - labelLength) / 2) - 2, UNDERLINE, item.label, RESET);
            return;
        }
        if (inputLength)
        {

            printf("\033[%d;%dH> %s: %s <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - labelLength) / 2) - 2, item.label, item.input);
        }
        else
        {

            printf("\033[%d;%dH> %s: %s<\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - labelLength) / 2) - 2, item.label, item.input);
        }
    }
    else
    {
        if (item.isCheckbox)
        {
            printf("\033[%d;%dH%s%s%s\n", (term_size.rows / 2) + offset,
                   (term_size.columns - labelLength) / 2, UNDERLINE, item.label, RESET);
            return;
        }

        printf("\033[%d;%dH%s: %s\n", (term_size.rows / 2) + offset,
               (term_size.columns - labelLength) / 2, item.label, item.input);
    }
}

void printMenuCheckbox(char *string, int32_t isSelected, int32_t isCheckboxSelected, int32_t offset)
{
    int stringLength = strlen(string);
    if (isSelected)
    {
        if (isCheckboxSelected)
        {
            printf("\033[%d;%dH> %s: [X] <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - stringLength) / 2) - 2, string);
        }
        else
        {
            printf("\033[%d;%dH> %s: [ ] <\n", (term_size.rows / 2) + offset,
                   ((term_size.columns - stringLength) / 2) - 2, string);
        }
    }
    else
    {
        if (isCheckboxSelected)
        {
            printf("\033[%d;%dH%s: [X]\n", (term_size.rows / 2) + offset,
                   (term_size.columns - stringLength) / 2, string);
        }
        else
        {
            printf("\033[%d;%dH%s: [ ]\n", (term_size.rows / 2) + offset,
                   (term_size.columns - stringLength) / 2, string);
        }
    }

    // int stringLength = strlen(string);
    // if (isSelected)
    // {
    //     printf("\033[%d;%dH> %s: [X] <\n", (term_size.rows / 2) + offset,
    //            ((term_size.columns - stringLength) / 2) - 2, string);
    // }
    // else
    // {
    //     printf("\033[%d;%dH%s: [ ]\n", (term_size.rows / 2) + offset,
    //            (term_size.columns - stringLength) / 2, string);
    // }
}

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
        {.label = "Nome", .input = "", .isCheckbox = 0},
        {.label = "Preço (€)", .input = "", .isCheckbox = 0},
        {.label = "Quantidade", .input = "", .isCheckbox = 0},
        {.label = "Categoria", .input = "", .isCheckbox = 1, .checkBoxOptions = {"Ramos", "Arranjos", "Jarros", "CentrosMesa", "OutrasFlores"}},
    };

    int32_t result = input_menu(inputItems, LENGTH(inputItems));
    switch (result)
    {
    case 0:
        size_artigos++;
        artigos = realloc(artigos, sizeof(Artigo) * (size_artigos));

        artigos[size_artigos - 1].nome = (char *)malloc(sizeof(char) * (strlen(inputItems[0].input) + 1));
        copy_str(artigos[size_artigos - 1].nome, inputItems[0].input, strlen(inputItems[0].input) + 1);

        artigos[size_artigos - 1].preco = atof(inputItems[1].input);
        artigos[size_artigos - 1].quantidade = atoi(inputItems[2].input);
        artigos[size_artigos - 1].categoria = atoi(inputItems[3].input);
        copy_str(artigos[size_artigos - 1].uuid, uuid_gen(), 37);

        clear_menu();
        menu_centered_item("Artigo introduzido com sucesso", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
        getchar();

        menu_principal();

        break;
    case 1:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}
void menu_listar(void)
{
    printf("Listar artigos\n");
}

void free_options(char **options, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(options[i]);
    }
    free(options);
}

void menu_modificar(void)
{
    if (size_artigos == 0)
    {
        clear_menu();
        menu_centered_item("Não há artigos para modificar", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
        getchar();
        menu_principal();
        return;
    }

    // use arrow_menu to select an already existing artigo
    // char *artigosOptions[size_artigos];
    char **artigosOptions = (char **)malloc(sizeof(char *) * size_artigos);
    if (artigosOptions == NULL)
    {
        fprintf(stderr, "Erro: malloc() retornou NULL\n");
        exit(1);
    }
    for (size_t i = 0; i < size_artigos; i++)
    {
        artigosOptions[i] = artigos[i].nome;
    }
    int32_t selectedArtigo = arrow_menu(artigosOptions, size_artigos);

    char preco[40];
    char quantidade[40];
    char nome[40];
    char categoria[40];
#ifdef _WIN32
    sprintf_s(preco, 40, "%.2f", artigos[selectedArtigo].preco);
    sprintf_s(nome, 40, "%s", artigos[selectedArtigo].nome);
    sprintf_s(quantidade, 40, "%lld", artigos[selectedArtigo].quantidade);
    sprintf_s(categoria, 40, "%d", artigos[selectedArtigo].categoria);

#elif __unix__
    sprintf(preco, "%.2f", artigos[selectedArtigo].preco);
    sprintf(nome, "%s", artigos[selectedArtigo].nome);
    sprintf(quantidade, "%ld", artigos[selectedArtigo].quantidade);
    sprintf(categoria, "%d", artigos[selectedArtigo].categoria);
#endif

    Input inputItems[] = {
        {.label = "Nome", .isCheckbox = 0},
        {.label = "Preço (€)", .isCheckbox = 0},
        {.label = "Quantidade", .isCheckbox = 0},
        {.label = "Categoria", .isCheckbox = 1, .checkBoxOptions = {"Ramos", "Arranjos", "Jarros", "CentrosMesa", "OutrasFlores"}},
    };

    // Copy the content from your character arrays to the input field

    copy_str(inputItems[0].input, nome, strlen(nome) + 1);
    copy_str(inputItems[1].input, preco, strlen(preco) + 1);
    copy_str(inputItems[2].input, quantidade, strlen(quantidade) + 1);
    copy_str(inputItems[3].input, categoria, strlen(categoria) + 1);

    int32_t result = input_menu(inputItems, LENGTH(inputItems));
    switch (result)
    {
    case 0:
        copy_str(artigos[selectedArtigo].nome, inputItems[0].input, strlen(inputItems[0].input) + 1);
        artigos[selectedArtigo].preco = atof(inputItems[1].input);
        artigos[selectedArtigo].quantidade = atoi(inputItems[2].input);
        artigos[selectedArtigo].categoria = atoi(inputItems[3].input);

        free_options(artigosOptions, size_artigos);
        clear_menu();
        menu_centered_item("Artigo modificado com sucesso", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
        getchar();
        menu_principal();
        break;
    case 1:
        free_options(artigosOptions, size_artigos);
        menu_principal();
        break;
    default:
        free_options(artigosOptions, size_artigos);
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
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