#include "main.h"
#include "colors.h"
#include "menu.h"
#include "term_size.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TerminalSize term_size;

void menu_gestao_clientes() {}
void menu_gestao_produtos() {}
void menu_gestao_vendas() {}
void menu_gestao_stocks() {}

void menu_principal(void)
{

  menu_centered_item("Introduzir informação sobre um artigo", RED, BOLD, 0);

  menu_centered_item("Listar informações", RED, BOLD, 1);

  menu_centered_item("Modificar o preço e/ou stock de um artigo", RED, BOLD, 2);

  menu_centered_item("Sair", RED, BOLD, 3);

  // get the user input
  // if the user input is 1, 2, 3, 4 or 5, call the corresponding function
  // otherwise, print an error message and call the main menu again
  char user_input;
  printf("\033[%d;%dH%s%s%s", term_size.rows - 1, 0, UNDERLINE,
         "Opção: ", RESET);
  scanf("%c", &user_input);
  switch (user_input)
  {
  case '1':
    clear_menu();
    menu_gestao_clientes();
    break;
  case '2':
    clear_menu();
    menu_gestao_produtos();
    break;
  case '3':
    clear_menu();
    menu_gestao_vendas();
    break;
  case '4':
    clear_menu();
    menu_gestao_stocks();
    break;
  case '5':
    clear_menu();
    exit(0);
    break;
  default:
    clear_menu();
    printf("\033[%d;%dH%s%s%s", term_size.rows / 2, term_size.columns / 2 - 10,
           RED, BOLD, "Opção inválida");
    getchar();
    clear_menu();
    menu_principal();
    break;
  }
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
  if (get_terminal_size(&term_size) != 0)
  {
    fprintf(stderr, "Erro a conseguir o tamanho do terminal...\n");
    return 1;
  }

  welcome_screen();
  return 0;
}
