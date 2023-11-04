#ifndef MAIN_H
#define MAIN_H

typedef enum
{
  Ramos,
  Arranjos,
  Jarros,
  CentrosMesa,
  OutasFlores
} CategoriaProduto;

typedef struct
{
  int codigo_produto;
  int preco;
  int quantidade;
  CategoriaProduto categoria;

} Artigo;

#endif