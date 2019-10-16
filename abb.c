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
/*Un entero menor que 0 si la primera cadena es menor que la segunda.
Un entero mayor que 0 si la primera cadena es mayor que la segunda.*/
n_abb_t* buscar_padre_clave(n_abb_t* raiz, const char *clave, int (*abb_comparar_clave_t) (const char *, const char *)){
    //reviso que el nodo exista y tenga hijos
    if (!raiz || (!raiz->izq && !raiz->der)) return;
    if (raiz->izq && raiz->izq->clave == clave || raiz->der && raiz->der->clave == clave)) return raiz;
    if (int (*abb_comparar_clave_t) ((raiz->izq->clave, clave))>0)buscar_padre_clave((raiz->izq, clave);
    else buscar_padre_clave((raiz->der, clave);
}

n_abb_t* buscar_clave(n_abb_t* nodo, const char *clave){
    if (!nodo) 
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if (!)
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!arbol->raiz) return;
    //busco el padre del nodo a borrar
    n_abb_t* padre_nodo_a_borrar = buscar_padre_clave(arbol->raiz, clave, arbol->abb_destruir_dato_t);
    if (!padre_nodo_a_borrar) return;
    n_abb_t* nodo_a_borrar =  padre_nodo_a_borrar->izq;
    if (arbol->abb_destruir_dato_t(padre_nodo_a_borrar->izq->clave,clave)==0) nodo_a_borrar = padre_nodo_a_borrar->der;
    if(!nodo_a_borrar) return;
    void* dato_a_devolver = nodo_a_borrar->dato;
    //caso nodo a borrar tiene ambos hijos
    if(nodo_a_borrar->izq && nodo_a_borrar->der){
        
    }
    //caso nodo sin hijos
    else if(!nodo_a_borrar->izq && !nodo_a_borrar->der){
        padre_nodo_a_borrar->izq = NULL;
        padre_nodo_a_borrar->der = NULL;
    }
    //caso nodo a borrar tiene un hijo
    else{
        if (arbol->abb_destruir_dato_t(padre_nodo_a_borrar->izq->clave, clave)
    }
    free(nodo_a_borrar);
    return dato_a_devolver;
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