#include "main.h"
#include "colors.h"
#include "term_size.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

TerminalSize term_size;

void menu_gestao_clientes() {}
void menu_gestao_produtos() {}
void menu_gestao_vendas() {}
void menu_gestao_stocks() {}

void menu_item(char *string, char *color1, char *color2)
{
  int32_t string_size = strlen(string);
  printf("\033[%d;%dH%s%s%s%s", term_size.rows, term_size.columns - string_size, color1,
         color2, string, RESET);
}

void menu_centered_item(char *string, char *color1,
                        char *color2, int32_t row_offset)
{

  int32_t string_size = strlen(string);
  printf("\033[%d;%dH%s%s%s%s", (term_size.rows / 2) + row_offset, (term_size.columns - string_size) / 2,
         color1, color2, string, RESET);
}

void menu_principal()
{

  menu_centered_item("Gestão de Clientes", RED, BOLD, 0);

  menu_centered_item("Gestão de Produtos", RED, BOLD, 1);

  menu_centered_item("Gestão de Vendas", RED, BOLD, 2);

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
    printf("\033[%d;%dH%s%s%s", term_size.rows / 2, term_size.columns / 2 - 10,
           RED, BOLD, "Opção inválida");
    getchar();
    system("clear");
    menu_principal();
    break;
  }
}

void welcome_screen()
{
  system("clear");
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
  system("clear");
  menu_principal();
}

int main(void)
{
  if (get_terminal_size(&term_size) == 0)
  {
    printf("Terminal size: %d rows, %d columns\n", term_size.rows,
           term_size.columns);
  }
  else
  {
    fprintf(stderr, "Error getting terminal size\n");
  }

  welcome_screen();
  return 0;
}
