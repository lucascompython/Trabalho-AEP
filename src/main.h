#ifndef MAIN_H
#define MAIN_H

#define LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define UNUSED(x) (void)(x)

#include <stdint.h>

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
  double preco;
  uint64_t quantidade;

} Artigo;
#endif