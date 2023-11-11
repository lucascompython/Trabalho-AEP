#ifndef MAIN_H
#define MAIN_H

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define UNUSED(x) (void)(x)

#ifdef _WIN32
#pragma warning(disable : 4820)
#endif

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
  char uuid[37];
  char *nome;
  CategoriaArtigo categoria;
  float preco;
  int quantidade;

} Artigo;
#endif