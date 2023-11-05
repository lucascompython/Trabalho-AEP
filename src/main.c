#include "main.h"
#include "colors.h"
#include "menu.h"
#include "term_size.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

TerminalSize term_size;

void menu_gestao_clientes() {}
void menu_gestao_produtos() {}
void menu_gestao_vendas() {}
void menu_gestao_stocks() {}

void menu_principal(void)
{

  char *options[] = {"Gestão de clientes", "Gestão de produtos",
                     "Gestão de vendas", "Gestão de stocks", "Sair"};

  int32_t totalOptions = (int)LENGTH(options);
  int32_t result = arrow_menu(options, totalOptions);
  printf("Numero selecionado: %d\n", result);
  getchar();
}

void welcome_screen(void)
{
  clear_menu();
  char *welcome_message = "Bem vindo ao sistema de gestão de stocks";
  int32_t welcome_message_size = strlen(welcome_message);
  int32_t welcome_message_row = term_size.rows / 2;
  int32_t welcome_message_column =
      (term_size.columns - welcome_message_size) / 2;
  printf("\033[%d;%dH%s%s%s%s", welcome_message_row, welcome_message_column,
         RED, BOLD, welcome_message, RESET);
  printf("\033[%d;%dH%s%s%s", welcome_message_row + 1, welcome_message_column,
         UNDERLINE, "Pressione qualquer tecla para continuar\n", RESET);
  getchar();
  clear_menu();
  menu_principal();
}

int main(void)
{
  setlocale(LC_ALL, "Portuguese.UTF8");

  if (get_terminal_size(&term_size) != 0)
  {
    fprintf(stderr, "Erro a conseguir o tamanho do terminal...\n");
    return 1;
  }

  welcome_screen();
  return 0;
}
