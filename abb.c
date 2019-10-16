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

/*Un entero menor que 0 si la primera cadena es menor que la segunda.
Un entero mayor que 0 si la primera cadena es mayor que la segunda.*/
n_abb_t* buscar_padre_clave(n_abb_t* raiz, const char *clave, int (*abb_comparar_clave_t) (const char *, const char *)){
    //reviso que el nodo exista y tenga hijos
    if (!raiz || (!raiz->izq && !raiz->der)) return NULL;
    //si es el padre de la clave que busco lo devuelvo
    if ((raiz->izq && raiz->izq->clave == clave) || (raiz->der && raiz->der->clave == clave)) return raiz;
    //en caso contrario, continuo iterando al lado que corresponda
    if (abb_comparar_clave_t(raiz->izq->clave, clave) > 0) return buscar_padre_clave(raiz->izq, clave, abb_comparar_clave_t);
    else return buscar_padre_clave(raiz->der, clave, abb_comparar_clave_t);
}

n_abb_t* buscar_padre(n_abb_t* nodo, n_abb_t* padre, const char *clave, int (*abb_comparar_clave_t)(const char *, const char *)){
    if (!nodo || !abb_comparar_clave_t(clave, nodo->clave)) return padre;
    if (abb_comparar_clave_t(clave, nodo->clave) > 0) return buscar_padre(nodo->der, nodo, clave, abb_comparar_clave_t);
    else return buscar_padre(nodo->izq, nodo, clave, abb_comparar_clave_t);
}

n_abb_t* buscar_nodo(n_abb_t* padre, int comp_claves, const abb_t* abb, const char* clave){
    if (!comp_claves){
        if (padre) comp_claves = abb->abb_comparar_clave_t(clave, padre->clave);
    }
    n_abb_t* nodo = abb->raiz;
    if (comp_claves && comp_claves < 0) nodo = padre->izq;
    if (comp_claves && comp_claves > 0) nodo = padre->der;
    return nodo;
}

n_abb_t* crear_nodo(const char* clave, void* dato){
    n_abb_t* nodo = malloc(sizeof(n_abb_t));
    char* _clave = strdup(clave);
    
    if (!_clave || !nodo){
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
    int comp_claves = 0;
    if (padre) comp_claves = arbol->abb_comparar_clave_t(clave, padre->clave);
    n_abb_t* nodo = buscar_nodo(padre, comp_claves, arbol, NULL);
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

void *abb_borrar(abb_t *arbol, const char *clave){
    if (!arbol->raiz) return NULL;
    //busco el padre del nodo a borrar
    n_abb_t* padre = buscar_padre_clave(arbol->raiz, clave, arbol->abb_comparar_clave_t);
    if (!padre) return NULL;
    n_abb_t* nodo_a_borrar =  padre->izq;
    if (arbol->abb_comparar_clave_t(nodo_a_borrar->clave,clave)!=0)nodo_a_borrar = padre->der;
    //si la clave no estaba en el arbol para este paso ya deja de ejecutarse
    if(!nodo_a_borrar) return NULL;
    //me guardo el dato para devolverlo despues de borrar el nodo
    void* dato_a_devolver = nodo_a_borrar->dato;
    n_abb_t* hijo_izq = nodo_a_borrar->izq;
    n_abb_t* hijo_der = nodo_a_borrar->der;
    //caso nodo a borrar tiene ambos hijos
    if(hijo_izq && hijo_der){
        n_abb_t* reemplazante = hijo_izq->der;
        if(!reemplazante) reemplazante = hijo_der->izq;
        //caso borde ninguno de los hijos tenia un reemplazante que me sirviera
        if(!reemplazante) reemplazante = hijo_izq;
        //guardo clave y dato del reemplazante y lo borro
        const char* clave_nueva = reemplazante->clave;
        void* dato_nuevo = abb_borrar(arbol,clave_nueva);
        nodo_a_borrar->clave = clave_nueva;
        nodo_a_borrar->dato = dato_nuevo;
        free(reemplazante);
        }
    //caso nodo sin hijos
    else if(!hijo_izq && !hijo_der){
        if (padre->izq == nodo_a_borrar) padre->izq = NULL;
        else padre->der = NULL;
    }
    //caso nodo a borrar tiene un hijo
    else{
        if (!hijo_izq) hijo_izq = nodo_a_borrar->der;
        if (padre->izq == nodo_a_borrar) padre->izq = hijo_izq;
        else padre->der = hijo_izq;
    }
    free(hijo_izq); free(hijo_der); free(nodo_a_borrar); free(padre);
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

void _abb_destruir(n_abb_t* nodo, void (*abb_destruir_dato_t) (void *)){
    if (!nodo) return NULL;
    //recorro post order
    _abb_destruir(nodo->izq, abb_destruir_dato_t);
    _abb_destruir(nodo->der, abb_destruir_dato_t);
    abb_destruir_dato_t(nodo->dato);
    free(nodo->clave);
    free(nodo);
}

void abb_destruir(abb_t *arbol){
    if (!arbol->raiz) return NULL;
    _abb_destruir(arbol->raiz, arbol->abb_destruir_dato_t);
    free(arbol);    
}

void _abb_in_order(n_abb_t *raiz, bool visitar(const char *, void *, void *), void *extra){
    if(!raiz) return NULL;
    _abb_in_order(raiz->izq, visitar, extra);
    if(!visitar(raiz->clave, raiz->dato, extra)) return NULL;
    _abb_in_order(raiz->der, visitar, extra);
}


void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    if(!arbol->raiz) return;
    _abb_in_order(arbol->raiz, visitar, extra);
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
    n_abb_t* nodo = pila_desapilar(iter->pila);
    if (nodo->der){
        if (!pila_apilar(iter->pila, nodo->der)) return false;
        apilar_izquierdos(nodo->der, iter->pila);
    }
    return true;        
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    return (((n_abb_t*)pila_ver_tope(iter->pila))->clave);
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}