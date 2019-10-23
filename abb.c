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
    char* clave;
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

/* Función aux para encontrar el padre de un nodo que contiene a la clave que busco
 * Si el valor devuelto es NULL, y el ABB tiene cantidad > 0, el nodo buscado es la raiz.
 */  
n_abb_t* buscar_padre(n_abb_t* nodo, n_abb_t* padre, const char *clave, abb_comparar_clave_t comparar_clave){
    if (!nodo || !comparar_clave(clave, nodo->clave)) return padre;
    if (comparar_clave(clave, nodo->clave) > 0) return buscar_padre(nodo->der, nodo, clave, comparar_clave);
    else return buscar_padre(nodo->izq, nodo, clave, comparar_clave);
}

n_abb_t* buscar_nodo(n_abb_t* padre, int comp_claves, const abb_t* abb, const char* clave){
    if (!comp_claves && padre) comp_claves = abb->abb_comparar_clave_t(clave, padre->clave);
    n_abb_t* nodo = NULL;
    if (!comp_claves) nodo = abb->raiz;
    if (comp_claves && comp_claves < 0) nodo = padre->izq;
    if (comp_claves && comp_claves > 0) nodo = padre->der;
    return nodo;
}

int comparar_claves(n_abb_t* padre, abb_t* arbol, const char* clave){
    int comp_claves = 0;
    if (padre) comp_claves = arbol->abb_comparar_clave_t(clave, padre->clave);
    return comp_claves;
}

n_abb_t* crear_nodo_abb(const char* clave, void* dato){
    n_abb_t* nodo = malloc(sizeof(n_abb_t));
    char* _clave = strdup(clave);

    if (!_clave || !nodo){
        free(_clave); 
        free(nodo);
        return NULL;
    }

    nodo->izq = NULL; nodo->der = NULL;
    nodo->clave = _clave;
    nodo->dato = dato;
    
    return nodo;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    n_abb_t* padre = buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t);

    int comp_claves = comparar_claves(padre, arbol, clave);

    n_abb_t* nodo = buscar_nodo(padre, comp_claves, arbol, NULL);
    if (!nodo){
        n_abb_t* nodo_nuevo = crear_nodo_abb(clave, dato);
        if (!nodo_nuevo) return false;
        if (comp_claves < 0) padre->izq = nodo_nuevo;
        else if (comp_claves > 0) padre->der = nodo_nuevo;
        else arbol->raiz = nodo_nuevo;
        arbol->cant++;
    }
    else{
        if (arbol->abb_destruir_dato_t) arbol->abb_destruir_dato_t(nodo->dato);
        nodo->dato = dato;
    }
    return true;
}

char* buscar_reemplazante(n_abb_t* nodo){
   if (!nodo->der) return nodo->clave;
   return buscar_reemplazante(nodo->der);
}

void borrar_dos_hijos(abb_t* arbol, n_abb_t* nodo, n_abb_t* hijo_izq){
    //guardo clave y dato del reemplazante y lo borro, luego actualizo
    char* clave_nueva = strdup(buscar_reemplazante(hijo_izq));
    void* dato_nuevo = abb_borrar(arbol, clave_nueva);
    free(nodo->clave);
    nodo->clave = clave_nueva;
    nodo->dato = dato_nuevo;
}

void borrar_sin_hijos(n_abb_t* padre, n_abb_t* nodo, int comp_claves, abb_t* arbol){
    if (comp_claves > 0) padre->der = NULL;
    else if (comp_claves < 0) padre->izq = NULL;
    else arbol->raiz = NULL;
    free(nodo->clave); 
    free(nodo);
    arbol->cant--;
}

void borrar_un_hijo(n_abb_t* padre, n_abb_t* hijo_izq, n_abb_t* nodo, int comp_claves, abb_t* arbol){
    if (!hijo_izq) hijo_izq = nodo->der;
    if (comp_claves > 0) padre->der = hijo_izq;
    else if (comp_claves < 0) padre->izq = hijo_izq;
    else arbol->raiz = hijo_izq;
    free(nodo->clave); 
    free(nodo);
    arbol->cant--;
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!arbol->cant) return NULL;

    n_abb_t* padre = buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t);
    int comp_claves = comparar_claves(padre, arbol, clave);

    n_abb_t* nodo_a_borrar = buscar_nodo(padre, comp_claves, arbol, NULL);
    if(!nodo_a_borrar) return NULL;

    void* dato_a_devolver = nodo_a_borrar->dato;

    n_abb_t* hijo_izq = nodo_a_borrar->izq; n_abb_t* hijo_der = nodo_a_borrar->der;
    if(hijo_izq && hijo_der) borrar_dos_hijos(arbol, nodo_a_borrar, hijo_izq);    
    else if(!hijo_izq && !hijo_der) borrar_sin_hijos(padre, nodo_a_borrar, comp_claves, arbol); 
    else borrar_un_hijo(padre, hijo_izq, nodo_a_borrar, comp_claves, arbol);

    return dato_a_devolver;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    n_abb_t* nodo = buscar_nodo(buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t), 0, arbol, clave);

    if (!nodo) return NULL;
    return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    n_abb_t* nodo = buscar_nodo(buscar_padre(arbol->raiz, NULL, clave, arbol->abb_comparar_clave_t), 0, arbol, clave);

    if (!nodo) return false;

    return true;
}

size_t abb_cantidad(abb_t *arbol){
    return arbol->cant;
}

void _abb_destruir(n_abb_t* nodo, abb_destruir_dato_t destruir_dato){
    if (!nodo) return;
    //recorro post order
    _abb_destruir(nodo->izq, destruir_dato);
    _abb_destruir(nodo->der, destruir_dato);
    if (destruir_dato) destruir_dato(nodo->dato);
    free(nodo->clave); 
    free(nodo);
}

void abb_destruir(abb_t *arbol){
    _abb_destruir(arbol->raiz, arbol->abb_destruir_dato_t);
    free(arbol);
}

bool _abb_in_order(n_abb_t *raiz, bool visitar(const char *, void *, void *), void *extra, bool cont_recorrido){
    if (!raiz || !cont_recorrido) return cont_recorrido;
    return _abb_in_order(raiz->izq, visitar, extra, cont_recorrido) && _abb_in_order(raiz->der, visitar, extra, visitar(raiz->clave, raiz->dato, extra)); 
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    bool cont_recorrido = true;
    _abb_in_order(arbol->raiz, visitar, extra, cont_recorrido);
}

struct abb_iter{
    pila_t* pila;
};

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
    if (abb_iter_in_al_final(iter)) return false;
    n_abb_t* nodo = pila_desapilar(iter->pila);
    if (nodo->der){
        if (!pila_apilar(iter->pila, nodo->der)) return false;
        apilar_izquierdos(nodo->der->izq, iter->pila);
    }
    return true;        
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (abb_iter_in_al_final(iter)) return NULL;
    return (((n_abb_t*)pila_ver_tope(iter->pila))->clave);
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}