#ifndef MAIN_H
#define MAIN_H

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define UNUSED(x) (void)(x)

typedef enum
{
  Ramos,
  Arranjos,
  Jarros,
  CentrosMesa,
  OutasFlores
} CategoriaArtigo;

typedef struct
{
  char *nome;
  char uuid[37];
  float preco;
  int quantidade;
  CategoriaArtigo categoria;

} Artigo;

#endif