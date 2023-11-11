#include "json.h"
#include "yyjson.h" // external/yyjson/src
#include "main.h"

#define JSON_FILE "db.json"

CategoriaArtigo str_to_categoria(const char *str)
{
    if (strcmp(str, "Ramos") == 0)
    {
        return Ramos;
    }
    else if (strcmp(str, "Arranjos") == 0)
    {
        return Arranjos;
    }
    else if (strcmp(str, "Jarros") == 0)
    {
        return Jarros;
    }
    else if (strcmp(str, "CentrosMesa") == 0)
    {
        return CentrosMesa;
    }
    else if (strcmp(str, "OutasFlores") == 0)
    {
        return OutasFlores;
    }
    else
    {
        fprintf(stderr, "Categoria inválida\n");
        exit(1);
    }
}

char *categoria_to_str(CategoriaArtigo categoria)
{
    switch (categoria)
    {
    case Ramos:
        return "Ramos";
    case Arranjos:
        return "Arranjos";
    case Jarros:
        return "Jarros";
    case CentrosMesa:
        return "CentrosMesa";
    case OutasFlores:
        return "OutasFlores";
    default:
        fprintf(stderr, "Categoria inválida\n");
        exit(1);
    }
}

void clean_artigos_array(Artigo *artigos, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(artigos[i].nome);
    }
    free(artigos);
}

Artigo *get_artigos_array(size_t *size_artigos)
{
    yyjson_doc *doc = yyjson_read_file(JSON_FILE, 0, NULL, NULL);
    if (!doc)
    {
        fprintf(stderr, "Erro ao ler o ficheiro JSON\n");
        exit(1);
    }

    yyjson_val *root = yyjson_doc_get_root(doc);

    size_t size = yyjson_obj_size(root);
    *size_artigos = size;

    Artigo *artigos = malloc(sizeof(Artigo) * size);
    if (artigos == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para o array de artigos\n");
        exit(1);
    }

    size_t idx, max;
    yyjson_val *key, *val;

    yyjson_obj_foreach(root, idx, max, key, val)
    {
        const char *nome = yyjson_get_str(key);
        const char *uuid = yyjson_get_str(yyjson_obj_get(val, "uuid"));
        double preco = yyjson_get_real(yyjson_obj_get(val, "preco"));
        int quantidade = yyjson_get_int(yyjson_obj_get(val, "quantidade"));
        const char *categoria = yyjson_get_str(yyjson_obj_get(val, "categoria"));

        artigos[idx].nome = (char *)malloc(sizeof(char) * strlen(nome) + 1);
        if (artigos[idx].nome == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o nome do artigo\n");
            exit(1);
        }
        strcpy_s(artigos[idx].nome, sizeof(nome), nome);

        strcpy_s(artigos[idx].uuid, 37, uuid);
        artigos[idx].preco = preco;
        artigos[idx].quantidade = quantidade;
        artigos[idx].categoria = str_to_categoria(categoria);
    }

    yyjson_doc_free(doc);
    return artigos;
}
