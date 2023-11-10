#ifndef JSON_H
#define JSON_H

#include "main.h"
#include <stdlib.h>

Artigo *get_artigos_array(size_t *size_artigos);
void clean_artigos_array(Artigo *artigos, size_t size);
CategoriaArtigo str_to_categoria(const char *str);
char *categoria_to_str(CategoriaArtigo categoria);

#endif // JSON_H
