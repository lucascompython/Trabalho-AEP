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
#include <math.h>

#ifdef _WIN32
#include <conio.h> // Para a welcome screen _getch() ler todas as teclas
#endif

extern TerminalSize term_size;       // from src/main.c
extern Artigo *artigos;              // from src/main.c
extern size_t size_artigos;          // from src/main.c
extern Artigo *artigos_vendidos;     // from src/main.c
extern size_t size_artigos_vendidos; // from src/main.c

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

    int32_t result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
        size_artigos++;
        artigos = realloc(artigos, sizeof(Artigo) * (size_artigos));
        if (artigos == NULL)
        {
            fprintf(stderr, "Erro: realloc() retornou NULL\n");
            exit(1);
        }

        artigos[size_artigos - 1].nome = (char *)malloc(sizeof(char) * (strlen(inputItems[0].input) + 1));
        if (artigos[size_artigos - 1].nome == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }
        copy_str(artigos[size_artigos - 1].nome, inputItems[0].input, strlen(inputItems[0].input) + 1);

        artigos[size_artigos - 1].preco = atof(inputItems[1].input);
        artigos[size_artigos - 1].quantidade = atoi(inputItems[2].input);
        artigos[size_artigos - 1].categoria = atoi(inputItems[3].input);
        copy_str(artigos[size_artigos - 1].uuid, uuid_gen(), 37);

        clear_menu();
        menu_centered_item("Artigo introduzido com sucesso!", GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // ler "qualquer" teclas no linux

        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif

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

    if (size_artigos == 0)
    {
        clear_menu();
        menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
#ifdef __unix__ // ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        return;
    }

    int32_t result = arrow_menu_filter(artigos, size_artigos);
    clear_menu();

    // show the selected artigo
    if (result != -1)
    {
        char preco[40];
        char quantidade[40];
        char nome[40];
        char categoria[40];
        char uuid[43];

#ifdef _WIN32
        sprintf_s(preco, 40, "Preço: %.2f", artigos[result].preco);
        sprintf_s(nome, 40, "Nome: %s", artigos[result].nome);
        sprintf_s(quantidade, 40, "Quantidade: %lld", artigos[result].quantidade);
        sprintf_s(categoria, 40, "Categoria: %d", artigos[result].categoria);
        sprintf_s(uuid, 43, "UUID: %s", artigos[result].uuid);
#elif __unix__
        sprintf(preco, "Preço: %.2f", artigos[result].preco);
        sprintf(nome, "Nome: %s", artigos[result].nome);
        sprintf(quantidade, "Quantidade: %ld", artigos[result].quantidade);
        sprintf(categoria, "Categoria: %s", categoria_to_str(artigos[result].categoria));
        sprintf(uuid, "UUID: %s", artigos[result].uuid);
#endif
        // TODO: Centrar isto melhor verticalmente
        menu_centered_item(nome, UNDERLINE, "", 0);
        menu_centered_item(preco, "", "", 1);
        menu_centered_item(quantidade, "", "", 2);
        menu_centered_item(categoria, "", "", 3);
        menu_centered_item(uuid, "", "", 4);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 5);
#ifdef __unix__ // ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
    }
    else
    {
        menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
    }
}

void menu_modificar(void)
{
    if (size_artigos == 0)
    {
        clear_menu();
        menu_centered_item("Não há artigos para modificar", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
#ifdef __unix__ // ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
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

    int32_t result = input_menu(inputItems, LENGTH(inputItems), 0);
    switch (result)
    {
    case 0:
        copy_str(artigos[selectedArtigo].nome, inputItems[0].input, strlen(inputItems[0].input) + 1);
        artigos[selectedArtigo].preco = atof(inputItems[1].input);
        artigos[selectedArtigo].quantidade = atoi(inputItems[2].input);
        artigos[selectedArtigo].categoria = atoi(inputItems[3].input);

        free(artigosOptions);
        clear_menu();
        menu_centered_item("Artigo modificado com sucesso!", GREEN, UNDERLINE, 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);
#ifdef __unix__ // ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        break;
    case 1:
        free(artigosOptions);
        menu_principal();
        break;
    default:
        free(artigosOptions);
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);
    }
}

int32_t get_category_count(void)
{
    int32_t categoryCount = 0;
    for (size_t i = 0; i < size_artigos; i++)
    {
        if ((int32_t)artigos[i].categoria > categoryCount)
        {
            categoryCount = artigos[i].categoria;
        }
    }
    return categoryCount + 1;
}

void menu_estatisticas(void)
{
    char *options[] = {
        "Stock por Categoria", // Alto e baixo preços | Quantidade maior e menor
        "Vendas",
        "Quebras", // Lixo | menu btop filtrar por categoria e total
        "Voltar"};

    int32_t totalOptions = (int)LENGTH(options);
    int32_t result = arrow_menu(options, totalOptions);
    switch (result)
    {
    case 0:
        if (size_artigos == 0)
        {
            clear_menu();
            menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
            enableRawMode();
            getchar();
            disableRawMode();
#elif _WIN32
            _getch(); // ler qualquer tecla no windows
#endif
            menu_principal();
            return;
        }
        cursor_upLeft();
        clear_menu();

        int32_t categoriaCount = get_category_count();

        int32_t *categoryCounts = (int32_t *)malloc(sizeof(int32_t) * categoriaCount);
        double *categoryTotalPrices = (double *)malloc(sizeof(double) * categoriaCount);
        if (categoryCounts == NULL || categoryTotalPrices == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }

        for (int32_t i = 0; i < categoriaCount; i++)
        {
            categoryCounts[i] = 0;
            categoryTotalPrices[i] = 0.0;
        }
        double precoTotal = 0.0;

        for (size_t i = 0; i < size_artigos; i++)
        {
            categoryCounts[artigos[i].categoria] += artigos[i].quantidade;
            for (uint64_t j = 0; j < artigos[i].quantidade; j++)
            {
                precoTotal += artigos[i].preco;
                categoryTotalPrices[artigos[i].categoria] += artigos[i].preco;
            }
        }

        char header[60];
#ifdef __unix__
        sprintf(header, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Preço");
#elif _WIN32
        sprintf_s(header, 60, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Preço");
#endif
        menu_centered_item(header, "", "", 0);

        char linhas[60] = "----------------------|------------|---------\n";
        menu_centered_item(linhas, "", "", 1);
        for (int32_t i = 0; i < categoriaCount; i++)
        {
            char *categoriaStr = categoria_to_str(i);
            char quantidade[40];
            char precoCategoriaStr[40];
            char linha[120];
#ifdef __unix__
            sprintf(quantidade, "%10d", categoryCounts[i]);
            sprintf(precoCategoriaStr, "%7.2f€", categoryTotalPrices[i]);
            sprintf(linha, "%-22s | %s | %s", categoriaStr, quantidade, precoCategoriaStr);
#elif _WIN32
            sprintf_s(quantidade, 40, "%10d", categoryCounts[i]);
            sprintf_s(precoCategoriaStr, 40, "%7.2f€", categoryTotalPrices[i]);
            sprintf_s(linha, 120, "%-22s | %s | %s", categoriaStr, quantidade, precoCategoriaStr);
#endif
            menu_centered_item(linha, "", "", i + 2);
        }

        menu_centered_item(linhas, "", "", categoriaCount + 2);
        // Print total price
        char precoTotalStr[40];
#ifdef __unix__
        sprintf(precoTotalStr, "Preço Total: %.2f\n", precoTotal);
#elif _WIN32
        sprintf_s(precoTotalStr, 40, "Preço Total: %.2f\n", precoTotal);
#endif
        menu_centered_item(precoTotalStr, BOLD, UNDERLINE, categoriaCount + 3);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", categoriaCount + 5);
#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();

        free(categoryCounts);
        free(categoryTotalPrices);

        // make a table with the quantity of each category plus the total

        break;
    case 1:
    {

        clear_menu();
        // Sem tempo para fazer isto direito :(
        if (size_artigos_vendidos == 0)
        {
            menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
            menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
            enableRawMode();
            getchar();
            disableRawMode();
#elif _WIN32
            _getch(); // ler qualquer tecla no windows

#endif
            menu_principal();
            break;
        }

        // Calcular media de todos os artigos vendidos

        double precoTotal = 0.0;
        for (size_t i = 0; i < size_artigos_vendidos; i++)
        {
            precoTotal += artigos_vendidos[i].preco * artigos_vendidos[i].quantidade;
        }

        double precoMedio = precoTotal / size_artigos_vendidos;

        char precoTotalStr[60];
#ifdef __unix__
        sprintf(precoTotalStr, "     Média dos preços dos artigos vendidos: %.2f€\n", precoMedio);

#elif _WIN32
        sprintf_s(precoTotalStr, 60, "     Média dos preços das vendas: %.2f€\n", precoMedio);
#endif

        menu_centered_item(precoTotalStr, BOLD, "", 0);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        break;
    }

    break;
    case 2:
    {
        clear_menu();
        int32_t categoriaCount = get_category_count();

        int32_t *categoryCounts = (int32_t *)malloc(sizeof(int32_t) * categoriaCount);
        double *categoryTotalPrices = (double *)malloc(sizeof(double) * categoriaCount);
        if (categoryCounts == NULL || categoryTotalPrices == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }

        for (int32_t i = 0; i < categoriaCount; i++)
        {
            categoryCounts[i] = 0;
            categoryTotalPrices[i] = 0.0;
        }

        for (size_t i = 0; i < size_artigos; i++)
        {
            categoryCounts[artigos[i].categoria] += artigos[i].quantidade;
            for (uint64_t j = 0; j < artigos[i].quantidade; j++)
            {
                categoryTotalPrices[artigos[i].categoria] += artigos[i].preco;
            }
        }

        int32_t *trashedCounts = (int32_t *)calloc(categoriaCount, sizeof(int32_t));

        double trashTotal = 0.0;
        for (int32_t i = 0; i < categoriaCount; i++)
        {
            double trashPercentage;
            switch (i)
            {
            case 0: // Ramos
                trashPercentage = 0.07;
                break;
            case 1: // Arranjos
                trashPercentage = 0.03;
                break;
            default:
                trashPercentage = 0.04;
                break;
            }
            trashedCounts[i] = (int32_t)round(categoryCounts[i] * trashPercentage);
            if (categoryCounts[i] != 0)
            {
                trashTotal += trashedCounts[i] * categoryTotalPrices[i] / categoryCounts[i];
            }
        }

        // Print the header of the table
        char header[60];
#ifdef __unix__
        sprintf(header, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Desperdício");
#elif _WIN32
        sprintf_s(header, 60, "%-20s | %10s | %7s\n", "Categoria", "Quantidade", "Desperdício");
#endif
        menu_centered_item(header, "", "", 0);

        // Print the line separator
        char linhas[60];
#ifdef __unix__
        sprintf(linhas, "--------------------|------------|------------\n");
#elif _WIN32
        sprintf_s(linhas, 60, "--------------------|------------|------------\n");
#endif
        menu_centered_item(linhas, "", "", 1);

        // Print each category with its count and total price
        for (int32_t i = 0; i < categoriaCount; i++)
        {
            char *categoriaStr = categoria_to_str(i);
            char quantidade[40];
            char desperdicio[40];
            char linha[120];
#ifdef __unix__
            sprintf(quantidade, "%10d", categoryCounts[i]);
            sprintf(desperdicio, "%10d", trashedCounts[i]);
            sprintf(linha, "%-17s | %s | %s", categoriaStr, quantidade, desperdicio);
#elif _WIN32
            sprintf_s(quantidade, 40, "%10d", categoryCounts[i]);
            sprintf_s(desperdicio, 40, "%10d", trashedCounts[i]);
            sprintf_s(linha, 120, "%-17s | %s | %s", categoriaStr, quantidade, desperdicio);
#endif
            menu_centered_item(linha, "", "", i + 2);
        }

        menu_centered_item(linhas, "", "", categoriaCount + 2);

        // Print total estimated loss
        char perdaEstimadaStr[40];
#ifdef __unix__
        sprintf(perdaEstimadaStr, "Perda Estimada: %.2f€\n", trashTotal);
#elif _WIN32
        sprintf_s(perdaEstimadaStr, 40, "Perda Estimada: %.2f€\n", trashTotal);
#endif
        menu_centered_item(perdaEstimadaStr, BOLD, UNDERLINE, categoriaCount + 3);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", categoriaCount + 5);
#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();

        free(categoryCounts);
        free(categoryTotalPrices);

        // make a table with the quantity of each category plus the total

        free(trashedCounts);
        break;
    }

    case 3:
        menu_principal();
        break;
    default:
        fprintf(stderr, "Erro: arrow_menu() retornou %d\n", result);
        exit(1);
        break;
    }
}
void menu_simular_vendas(void)
{

    if (size_artigos == 0)
    {
        clear_menu();
        menu_centered_item("Não há artigos para listar", UNDERLINE, "", 0);
        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 1);

#ifdef __unix__ // temos que fazer isto para ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
        disableRawMode();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
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

    int32_t result = input_menu(inputItems, LENGTH(inputItems), 1);
    switch (result)
    {
    case 0:
        // realloc artigos_vendidos
        size_artigos_vendidos++;
        artigos_vendidos = realloc(artigos_vendidos, sizeof(Artigo) * (size_artigos_vendidos));
        if (artigos_vendidos == NULL)
        {
            fprintf(stderr, "Erro: realloc() retornou NULL\n");
            exit(1);
        }

        // copy the selected artigo to artigos_vendidos
        artigos_vendidos[size_artigos_vendidos - 1].nome = (char *)malloc(sizeof(char) * (strlen(artigos[selectedArtigo].nome) + 1));
        if (artigos_vendidos[size_artigos_vendidos - 1].nome == NULL)
        {
            fprintf(stderr, "Erro: malloc() retornou NULL\n");
            exit(1);
        }

        copy_str(artigos_vendidos[size_artigos_vendidos - 1].nome, artigos[selectedArtigo].nome, strlen(artigos[selectedArtigo].nome) + 1);
        artigos_vendidos[size_artigos_vendidos - 1].preco = artigos[selectedArtigo].preco;
        artigos_vendidos[size_artigos_vendidos - 1].quantidade = atoi(inputItems[2].input);
        artigos_vendidos[size_artigos_vendidos - 1].categoria = artigos[selectedArtigo].categoria;
        copy_str(artigos_vendidos[size_artigos_vendidos - 1].uuid, uuid_gen(), 37);

        // subtract the quantity from the selected artigo
        artigos[selectedArtigo].quantidade -= atoi(inputItems[2].input);

        // save the artigos array to the json file
        save_artigos_array(artigos, size_artigos, STOCK_JSON_FILE);

        // save the artigos_vendidos array to the json file
        save_artigos_array(artigos_vendidos, size_artigos_vendidos, VENDAS_JSON_FILE);

        free(artigosOptions);
        clear_menu();

        menu_centered_item("Venda simulada com sucesso!", GREEN, UNDERLINE, 0);
        // total a pagar

        char precoTotalStr[40];
#ifdef __unix__
        sprintf(precoTotalStr, "Total a pagar: %.2f€\n", artigos_vendidos[size_artigos_vendidos - 1].preco * artigos_vendidos[size_artigos_vendidos - 1].quantidade);
#elif _WIN32
        sprintf_s(precoTotalStr, 40, "Total a pagar: %.2f€\n", artigos_vendidos[size_artigos_vendidos - 1].preco * artigos_vendidos[size_artigos_vendidos - 1].quantidade);
#endif
        menu_centered_item(precoTotalStr, BOLD, UNDERLINE, 1);

        menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "", 2);
#ifdef __unix__ // ler "qualquer" teclas no linux
        enableRawMode();
        getchar();
#elif _WIN32
        _getch(); // ler qualquer tecla no windows
#endif
        menu_principal();
        break;
    case 1:
        free(artigosOptions);
        menu_principal();
        break;
    default:
        free(artigosOptions);
        fprintf(stderr, "Erro: input_menu() retornou %d\n", result);
        exit(1);

        break;
    }
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