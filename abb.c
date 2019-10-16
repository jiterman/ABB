#define _GNU_SOURCE 1
#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct n_abb{
    struct n_abb* izq;
    struct n_abb* der;
    void* dato;
    const char* clave;
} n_abb_t;

struct abb{
    n_abb_t* raiz;
    int (*abb_comparar_clave_t) (const char *, const char *);
    void (*abb_destruir_dato_t) (void *);
    size_t cant;
}

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;

    abb->abb_comparar_clave_t = cmp;
    abb->abb_destruir_dato_t = destruir_dato;
    abb->raiz = NULL;
    abb->cant = 0;

    return abb;
}

n_abb_t* buscar_padre_clave(n_abb_t* raiz, const char *clave){
    //reviso que el nodo exista y tenga hijos
    if (!raiz || (!raiz->izq && !raiz->der)) return;
    if (raiz->izq && raiz->izq->clave == clave || raiz->der && raiz->der->clave == clave)) return raiz;
    buscar_padre_clave((raiz->izq, clave);
    buscar_padre_clave((raiz->der, clave);
}

n_abb_t* buscar_clave(n_abb_t* nodo, const char *clave){
    if (!nodo) 
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!)
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!arbol->raiz) return;
    n_abb_t* padre_nodo_a_borrar;
    if (arbol->raiz->clave != clave) {
        padre_nodo_a_borrar = buscar_padre_clave(arbol->raiz, clave);
        if (!padre_nodo_a_borrar) return;
    }
    n_abb_t* nodo_a_borrar = padre_nodo_a_borrar->izq;
    if (padre_nodo_a_borrar->izq->clave != clave) nodo_a_borrar = padre_nodo_a_borrar->der;
    //caso nodo a borrar tiene ambos hijos
    if(nodo_a_borrar->izq && nodo_a_borrar->der)
    //caso nodo sin hijos

    //caso nodo a borrar tiene un hijo

}

void *abb_obtener(const abb_t *arbol, const char *clave);
bool abb_pertenece(const abb_t *arbol, const char *clave);

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void abb_destruir(abb_t *arbol);

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

typedef struct abb_iter abb_iter_t;

abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
bool abb_iter_in_avanzar(abb_iter_t *iter);
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
bool abb_iter_in_al_final(const abb_iter_t *iter);
void abb_iter_in_destruir(abb_iter_t* iter);