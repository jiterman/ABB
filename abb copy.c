#define _GNU_SOURCE 1
#include "abb.h"
#include "pila.h"
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
};

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;

    abb->abb_comparar_clave_t = cmp;
    abb->abb_destruir_dato_t = destruir_dato;
    abb->raiz = NULL;
    abb->cant = 0;

    return abb;
}

// n_abb_t* buscar_padre_clave(n_abb_t* raiz, const char *clave){
//     //reviso que el nodo exista y tenga hijos
//     if (!raiz || (!raiz->izq && !raiz->der)) return;
//     if (raiz->izq && raiz->izq->clave == clave || raiz->der && raiz->der->clave == clave)) return raiz;
//     buscar_padre_clave((raiz->izq, clave);
//     buscar_padre_clave((raiz->der, clave);
// }

n_abb_t* buscar_padre(n_abb_t* nodo, n_abb_t* padre, const char *clave, int (*abb_comparar_clave_t)(const char *, const char *)){
    if (!nodo || !abb_comparar_clave_t(clave, nodo->clave)) return padre;
    if (abb_comparar_clave_t(clave, nodo->clave) > 0) return buscar_clave(nodo->der, nodo, clave, abb_comparar_clave_t);
    if (abb_comparar_clave_t(clave, nodo->clave) < 0) return buscar_clave(nodo->izq, nodo, clave, abb_comparar_clave_t);
}

n_abb_t* buscar_nodo(n_abb_t* padre, int comp_claves, abb_t* abb){
    if (!comp_claves){
        int comp_claves = NULL;
        if (padre) comp_claves = abb_comparar_clave_t(clave, padre->clave);
    }
    n_abb_t nodo = abb->raiz;
    if (comp_claves < 0) nodo = padre->izq;
    if (comp_claves > 0) nodo = padre->der;
    return nodo;
}

n_abb_t* crear_nodo(const char* clave, void* dato){
    n_abb_t* nodo = malloc(sizeof(n_abb_t));
    char* _clave = strdup(clave);
    
    if (!_clavep || !nodo){
        free(_clave); free(nodo);
        return NULL;
    }

    nodo->izq = NULL; nodo->der = NULL;
    nodo->clave = _clave;
    nodo->dato = dato;
    
    return nodo;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    n_abb_t* padre = buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t);
    int comp_claves = NULL;
    if (padre) comp_claves = abb_comparar_clave_t(clave, padre->clave);
    n_abb_t nodo = buscar_nodo(padre, comp_claves, arbol);
    if (!nodo){
        n_abb_t* nodo_nuevo = crear_nodo(clave, dato);
        if (!nodo_nuevo) return false;
        if (comp_claves < 0) padre->izq = nodo_nuevo;
        if (comp_claves > 0) padre->der = nodo_nuevo;
        else arbol->raiz = nodo;
        arbol->cant++;
        }
    else{
        if (arbol->abb_destruir_dato_t) arbol->abb_destruir_dato_t(nodo->dato);
        nodo->dato = dato;
    }
    return true;
    }
}

// void *abb_borrar(abb_t *arbol, const char *clave){
//     if (!arbol->raiz) return;
//     n_abb_t* padre_nodo_a_borrar;
//     if (arbol->raiz->clave != clave) {
//         padre_nodo_a_borrar = buscar_padre_clave(arbol->raiz, clave);
//         if (!padre_nodo_a_borrar) return;
//     }
//     n_abb_t* nodo_a_borrar = padre_nodo_a_borrar->izq;
//     if (padre_nodo_a_borrar->izq->clave != clave) nodo_a_borrar = padre_nodo_a_borrar->der;
//     //caso nodo a borrar tiene ambos hijos
//     if(nodo_a_borrar->izq && nodo_a_borrar->der)
//     //caso nodo sin hijos

//     //caso nodo a borrar tiene un hijo

// }

void *abb_obtener(const abb_t *arbol, const char *clave){
    n_abb_t* nodo = buscar_nodo(buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t), NULL, arbol);
    
    if (!nodo) return NULL;
    
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    n_abb_t* nodo = buscar_nodo(buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t), NULL, arbol);

    if (!nodo) return false;

    return true;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void abb_destruir(abb_t *arbol);

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

struct abb_iter{
    pila_t* pila;
}

void apilar_izquierdos(n_abb_t* nodo, pila_t* pila){
    if (!nodo) return;
    pila_apilar(pila, nodo);
    apilar_izquierdos(nodo->izq, pila);
}

abb_iter_t* abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;
    iter->pila = pila_crear();
    if (!iter->pila) return NULL;

    apilar_izquierdos(arbol->raiz, iter->pila);

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    n_abb_t* nodo = pila_desapilar(iter->pila);
    if (nodo->der){
        if (!pila_apilar(iter->pila, nodo->der)) return false;
        apilar_izquierdos(nodo->der, iter->pila);
    }
    return true;        
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    return (pila_ver_tope(iter->pila)->clave);
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}