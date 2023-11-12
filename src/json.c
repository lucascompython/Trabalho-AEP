#include "json.h"
#include "yyjson.h" // external/yyjson/src
#include "main.h"

#define STOCK_JSON_FILE "stock.json"
#define VENDAS_JSON_FILE "vendas.json"

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

void copy_str(char *dest, const char *src, size_t size)
{
#ifdef __unix__
    strncpy(dest, src, size);
#elif _WIN32
    strcpy_s(dest, size, src);
#endif
}

Artigo *get_artigos_array(size_t *size_artigos)
{
    yyjson_doc *doc = yyjson_read_file(STOCK_JSON_FILE, 0, NULL, NULL);
    if (!doc)
    {
        fprintf(stderr, "Erro ao ler o ficheiro JSON\n");
        exit(1);
    }

    yyjson_val *root = yyjson_doc_get_root(doc);

    size_t size = yyjson_obj_size(root);
    *size_artigos = size;

    Artigo *artigos = (Artigo *)malloc(sizeof(Artigo) * size);
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
        const size_t nome_size = sizeof(char) * strlen(nome) + 1; // 1 * strlen(nome) + 1 (terminador de string)
        const char *uuid = yyjson_get_str(yyjson_obj_get(val, "uuid"));
        double preco = yyjson_get_num(yyjson_obj_get(val, "preco"));
        uint64_t quantidade = yyjson_get_uint(yyjson_obj_get(val, "quantidade"));
        const char *categoria = yyjson_get_str(yyjson_obj_get(val, "categoria"));

        artigos[idx].nome = (char *)malloc(nome_size);
        if (artigos[idx].nome == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para o nome do artigo\n");
            exit(1);
        }

        copy_str(artigos[idx].nome, nome, nome_size);
        copy_str(artigos[idx].uuid, uuid, 37);

        artigos[idx].preco = preco;
        artigos[idx].quantidade = quantidade;
        artigos[idx].categoria = str_to_categoria(categoria);
    }
    yyjson_doc_free(doc);
    return artigos;
}

void save_artigos_array(Artigo *artigos, size_t size)
{
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *root = yyjson_mut_obj(doc);

    for (size_t i = 0; i < size; i++)
    {
        yyjson_mut_val *key = yyjson_mut_strn(doc, artigos[i].nome, strlen(artigos[i].nome));
        yyjson_mut_val *obj = yyjson_mut_obj(doc);

        yyjson_mut_val *uuid = yyjson_mut_strn(doc, artigos[i].uuid, 36);
        yyjson_mut_val *preco = yyjson_mut_real(doc, artigos[i].preco);
        yyjson_mut_val *quantidade = yyjson_mut_uint(doc, artigos[i].quantidade);
        yyjson_mut_val *categoria = yyjson_mut_strn(doc, categoria_to_str(artigos[i].categoria), strlen(categoria_to_str(artigos[i].categoria)));

        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "uuid"), uuid);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "preco"), preco);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "quantidade"), quantidade);
        yyjson_mut_obj_add(obj, yyjson_mut_str(doc, "categoria"), categoria);

        yyjson_mut_obj_add(root, key, obj);
    }

    yyjson_mut_doc_set_root(doc, root);

    yyjson_mut_write_file(STOCK_JSON_FILE, doc, YYJSON_WRITE_PRETTY, NULL, NULL);
    yyjson_mut_doc_free(doc);
}
