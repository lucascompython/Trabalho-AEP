#include "main.h"
#include "colors.h"
#include "menu.h"
#include "term_size.h"
#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TerminalSize term_size;

void menu_gestao_clientes() {
  menu_centered_item("Gestão de clientes", BLUE, BOLD, 0);
}
void menu_gestao_produtos() {
  menu_centered_item("Gestão de produtos", BLUE, BOLD, 0);
}
void menu_gestao_vendas() {
  menu_centered_item("Gestão de vendas", BLUE, BOLD, 0);
}
void menu_gestao_stocks() {
  menu_centered_item("Gestão de stocks", BLUE, BOLD, 0);
}

void menu_principal(void) {

  char *options[] = {"Gestão de clientes", "Gestão de produtos",
                     "Gestão de vendas", "Gestão de stocks", "Sair"};

  int32_t totalOptions = (int)LENGTH(options);
  int32_t result = arrow_menu(options, totalOptions);
  clear_menu();
  switch (result) {
  case 0:
    menu_gestao_clientes();
    break;
  case 1:
    menu_gestao_produtos();
    break;
  case 2:
    menu_gestao_vendas();
    break;
  case 3:
    menu_gestao_stocks();
    break;
  case 4:
    exit(0);
    break;
  default:
    exit(1);
    break;
  }
}

void welcome_screen(void) {
  clear_menu();

  menu_centered_item("Bem vindo ao sistema de gestão de stocks", GREEN, BOLD,
                     0);
  menu_centered_item("Pressione qualquer tecla para continuar", UNDERLINE, "",
                     1);
  puts("");

  getchar();
  clear_menu();
  menu_principal();
}

int main(void) {
  setlocale(LC_ALL, "Portuguese.UTF8");

  hide_cursor();

  if (get_terminal_size(&term_size) != 0) {
    fprintf(stderr, "Erro a conseguir o tamanho do terminal...\n");
    return 1;
  }

  welcome_screen();
  show_cursor();
  return 0;
}
