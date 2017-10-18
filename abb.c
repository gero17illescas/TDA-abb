#include "abb.h"
/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb_nodo {
	struct abb_nodo *izq;
	struct abb_nodo *der;
	char *clave;
	void *dato;
} abb_nodo_t;

typedef struct abb {
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cant_nodos;
}abb_t;


/* *****************************************************************
 *                DEFINICION DE FUNCIONES AUXILIARES
 * *****************************************************************/
abb_nodo_t* crear_nodo_abb(char* clave,void* dato){
    abb_nodo_t* nodo_abb = malloc(sizeof(abb_nodo_t));
    if (!nodo_abb) return NULL;

    nodo_abb->izq = NULL;
    nodo_abb->der = NULL;
    nodo_abb->dato = dato;
    nodo_abb->clave = malloc(sizeof(char)*strlen(clave));

    if(! nodo_abb->clave){
        free(nodo_abb);
        return NULL;
    }

    strcpy(nodo_abb->clave,clave);
    return nodo_abb;
}
 
/* *****************************************************************
 *                     PRIMITIVAS DEL ABB
 * *****************************************************************/
/* Recibe funciones para comparar los datos entre si y para destruirlos
 * Devuelve un puntero a un arbol binario vacio (raiz nula)
 * Pre: cmp y destruir_dato fueron previamente definidas.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t * arbol = malloc(sizeof(abb_t));
	if(!arbol) return NULL;
	arbol->raiz = NULL;
	arbol->destruir_dato = destruir_dato;
	arbol->cmp = cmp;
	return arbol;
}

/* Reciben un valor y una clave asociada al valor y lo guarda en el arbol.
 * Devuleve un boolenao si la operacion fue definida.
 * Pre: el arbol fue previamente creado.
 */


bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz){
        arbol->raiz = crear_nodo_abb(clave,dato);
        if(!arbol->raiz) return false;
    }

    if(arbol->raiz<clave)
        return abb_guardar(arbol->izq,clave,dato);

    if(arbol->raiz>clave)
        return abb_guardar(arbol->der,clave,dato);

    if(arbol->raiz==clave){
        arbol->destruir_dato(arbol->raiz->dato);
        arbol->raiz->dato = dato;
        return true;
    }
}

/*
 *
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/*
 *
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/*
 *
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/*
 *
 */
size_t abb_cantidad(abb_t *arbol);

/*
 *
 */
void abb_destruir(abb_t *arbol);

