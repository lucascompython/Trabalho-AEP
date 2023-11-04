#include "main.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

void menu_gestao_clientes() {}
void menu_gestao_produtos() {}
void menu_gestao_vendas() {}
void menu_gestao_stocks() {}

typedef struct
{
  int32_t rows;
  int32_t columns;
} TerminalSize;

TerminalSize get_terminal_size(void)
{
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  TerminalSize size = {w.ws_row, w.ws_col};
  return size;
}

void menu_item(char *string, int32_t string_size, TerminalSize size,
               char *color1, char *color2)
{
  printf("\033[%d;%dH%s%s%s%s", size.rows, size.columns - string_size, color1,
         color2, string, RESET);
}

void menu_principal(TerminalSize size)
{

  char *gestao_clientes = "Gestão de Clientes";
  int32_t gestao_clientes_size = strlen(gestao_clientes);
  int32_t gestao_clientes_row = size.rows / 2;
  int32_t gestao_clientes_column = (size.columns - gestao_clientes_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", gestao_clientes_row, gestao_clientes_column,
         RED, BOLD, gestao_clientes, RESET);

  char *gestao_produtos = "Gestão de Produtos";
  int32_t gestao_produtos_size = strlen(gestao_produtos);
  int32_t gestao_produtos_row = size.rows / 2 + 1;
  int32_t gestao_produtos_column = (size.columns - gestao_produtos_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", gestao_produtos_row, gestao_produtos_column,
         RED, BOLD, gestao_produtos, RESET);

  char *gestao_vendas = "Gestão de Vendas";
  int32_t gestao_vendas_size = strlen(gestao_vendas);
  int32_t gestao_vendas_row = size.rows / 2 + 2;
  int32_t gestao_vendas_column = (size.columns - gestao_vendas_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", gestao_vendas_row, gestao_vendas_column, RED,
         BOLD, gestao_vendas, RESET);

  char *gestao_stocks = "Gestão de Stocks";
  int32_t gestao_stocks_size = strlen(gestao_stocks);
  int32_t gestao_stocks_row = size.rows / 2 + 3;
  int32_t gestao_stocks_column = (size.columns - gestao_stocks_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", gestao_stocks_row, gestao_stocks_column, RED,
         BOLD, gestao_stocks, RESET);

  char *sair = "Sair";
  int32_t sair_size = strlen(sair);
  int32_t sair_row = size.rows / 2 + 4;
  int32_t sair_column = (size.columns - sair_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", sair_row, sair_column, RED, BOLD, sair, RESET);

  // get the user input
  // if the user input is 1, 2, 3, 4 or 5, call the corresponding function
  // otherwise, print an error message and call the main menu again
  char user_input;
  printf("\033[%d;%dH%s%s%s", size.rows - 1, 0, UNDERLINE, "Opção: ", RESET);
  scanf("%c", &user_input);
  switch (user_input)
  {
  case '1':
    system("clear");
    menu_gestao_clientes();
    break;
  case '2':
    system("clear");
    menu_gestao_produtos();
    break;
  case '3':
    system("clear");
    menu_gestao_vendas();
    break;
  case '4':
    system("clear");
    menu_gestao_stocks();
    break;
  case '5':
    system("clear");
    exit(0);
    break;
  default:
    system("clear");
    printf("\033[%d;%dH%s%s%s", size.rows / 2, size.columns / 2 - 10, RED, BOLD,
           "Opção inválida");
    getchar();
    menu_principal(size);
    break;
  }
}

void welcome_screen(TerminalSize size)
{
  // calculate the size of the terminal and print the welcome message at the
  // center
  system("clear");
  char *welcome_message = "Bem vindo ao sistema de gestão de stocks";
  int32_t welcome_message_size = strlen(welcome_message);
  int32_t welcome_message_row = size.rows / 2;
  int32_t welcome_message_column = (size.columns - welcome_message_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", welcome_message_row, welcome_message_column,
         RED, BOLD, welcome_message, RESET);
  printf("\033[%d;%dH%s%s%s", welcome_message_row + 1, welcome_message_column,
         UNDERLINE, "Pressione qualquer tecla para continuar\n", RESET);
  getchar();
  system("clear");
  menu_principal(size);
}

int main(void)
{
  TerminalSize size = get_terminal_size();

  welcome_screen(size);
  return 0;
}
