#include "abb.h"
#include "math.h"
#include <stdlib.h>
#include <string.h>
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

typedef struct iter_abb{
    abb_t* arbol;
    abb_nodo_t* nodo;
    size_t iterados;
}iter_abb_t;

/* *****************************************************************
 *                DEFINICION DE FUNCIONES AUXILIARES
 * *****************************************************************/
/* Crea un nodo para un arbol binario de busqueda. Si fallo el pedido
 * de memoria devuleve NULL.
 */
abb_nodo_t* crear_nodo_abb(const char* clave,void* dato){
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
/* Recibe un nodo de arbol binario de busqueda, un clave y una fucnion
 * de comparacion. Busca en los subarboles del nodo el nodo identificado
 * con la clave pasada por parametro. Si no se encuntra devuelve NULL.
 * Pre: cmp fue definida.
 */
abb_nodo_t* abb_nodo_buscar(abb_nodo_t*nodo,const char* clave,abb_comparar_clave_t cmp){
    if(!nodo)
        return NULL;
    if(cmp(nodo->clave,clave)==0)
        return nodo;
    if(cmp(nodo->clave,clave)<0)
        return abb_nodo_buscar(nodo->izq,clave,cmp);
    if(cmp(nodo->clave,clave)>0)
        return abb_nodo_buscar(nodo->der,clave,cmp);
    return NULL;
}

/* Recibe un nodo de arbol binario de busqueda, un clave, un nodo 
 * anterior y una fucnion de comparacion. Busca en los subarboles del 
 * nodo el nodo identificado con la clave pasada por parametro y devuelve
 * el nodo predecesor.Si no se encuntra devuelve NULL.
 * Pre: cmp fue definida.
 */
abb_nodo_t* _abb_nodo_buscar_predecesor(abb_nodo_t*nodo,abb_nodo_t*predecesor,const char* clave,abb_comparar_clave_t cmp){
    if(!nodo)
        return NULL;
    if(cmp(nodo->clave,clave)==0)
        return predecesor;
    if(cmp(nodo->clave,clave)<0)
        return _abb_nodo_buscar_predecesor(nodo->izq,nodo,clave,cmp);
    if(cmp(nodo->clave,clave)>0)
        return _abb_nodo_buscar_predecesor(nodo->der,nodo,clave,cmp);
        return NULL;
}

abb_nodo_t* abb_nodo_buscar_predecesor(abb_t* arbol,const char* clave){
    return _abb_nodo_buscar_predecesor(arbol->raiz,NULL,clave,arbol->cmp);
}
/* Busca el nodo cuya clave sea la minima en ambos subarboles del nodo
 * pasadoa por parametro. Si no se encuntra devuelve NULL;
 */
abb_nodo_t* abb_nodo_buscar_minimo(abb_nodo_t*nodo){
    if(!nodo) return NULL;
    if(nodo->izq)
        return abb_nodo_buscar_minimo(nodo->izq);
    if(nodo->der)
        return abb_nodo_buscar_minimo(nodo->der);
    return nodo;
}
/* Calcula de manera recursiva la cantidad de niveles que tiene el arbol
 * del nodo pasador por parametro.
 */

size_t calcular_altura(abb_nodo_t*nodo){
    if(!nodo) return 0;
    size_t altura_izq = calcular_altura(nodo->izq);
    size_t altura_der = calcular_altura(nodo->der);
    if(altura_der>altura_izq)
        return 1+altura_der;
    return 1+altura_izq;
    
}
/* Devulve un bool segun si el arbol esta balanceado o no.Esta balanceado
 *  cuando las diferencias de altura de los subarboles es menor que 1.
 */
//bool esta_balanceado(abb_nodo_t*nodo){
//    if(!arbol->raiz) return true;
//    return fabsf(calcular_altura(arbol->raiz->izq)-calcular_altura(arbol->raiz->der))>=1;
//}
 
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
    arbol->cant_nodos = 0;
    return arbol;
}

/* Reciben un valor y una clave asociada al valor y lo guarda en el arbol.
 * Devuleve un boolenao si la operacion fue definida.
 * Pre: el arbol fue previamente creado.
 */

bool _abb_guardar(abb_nodo_t *nodo,abb_nodo_t *predecesor,abb_t *arbol, const char *clave, void *dato){
    if(!nodo){
        nodo = crear_nodo_abb(clave,dato);
        if(!nodo) return false;
        if(arbol->cmp(predecesor->clave,clave)<0)
            predecesor->der = nodo; 
        else
            predecesor->izq = nodo; 
        arbol->cant_nodos++;
        return true;
    }

    if(arbol->cmp(nodo->clave,clave)<0)
        return _abb_guardar(nodo->izq,nodo,arbol,clave,dato);

    if(arbol->cmp(nodo->clave,clave)>0)
        return _abb_guardar(nodo->der,nodo,arbol,clave,dato);

    if(arbol->cmp(nodo->clave,clave)==0){
        arbol->destruir_dato(nodo->dato);
        nodo->dato = dato;
        return true;
    }
}
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz){
        arbol->raiz = crear_nodo_abb(clave,dato);
        arbol->cant_nodos++;
        if(!arbol->raiz) return false;
        return true;
    }
    if(!_abb_guardar(arbol->raiz,NULL,arbol,clave,dato))
        return false;
    arbol->cant_nodos++;
    return true;
    
}

/* Recibe una clave y se encarga de eliminar el dato asociado y lo 
 * devuelve. Si la clave no pertenece a ningun dato del arbol, o este
 * esta vacio se devuelve NULL.
 * Pre: el arbol fue creado.
 */
void *abb_borrar(abb_t *arbol, const char *clave){
    abb_nodo_t* nodo = abb_nodo_buscar(arbol->raiz,clave,arbol->cmp);
    abb_nodo_t* predecesor = abb_nodo_buscar_predecesor(arbol,clave);
    if(!nodo) return NULL;
    if(predecesor){
        //no tiene hijos
        if(!nodo->izq && !nodo->der){
            if(predecesor->izq == nodo)
                predecesor->izq = NULL;
            else
                predecesor->der = NULL;
        }
        //tiene solo hijo izquierdo
        if(nodo->izq && !nodo->der){
            if(predecesor->izq == nodo)
                predecesor->izq = nodo->izq;
            if(predecesor->der == nodo)
                predecesor->der = nodo->izq;
        }
        //tiene solo hijo derecho
        if(!nodo->izq && nodo->der){
            if(predecesor->izq == nodo)
                predecesor->izq = nodo->der;
            if(predecesor->der == nodo)
                predecesor->der = nodo->der;
        }
        //tiene ambos hijos
        if(nodo->izq && nodo->der){
            abb_nodo_t* minimo_der = abb_nodo_buscar_minimo(nodo->der);
            if(predecesor->izq == nodo)
                predecesor->izq = minimo_der;
            if(predecesor->der == nodo)
                predecesor->der = minimo_der;
        }
    }
    else
        arbol->raiz = abb_nodo_buscar_minimo(nodo->der);
    arbol->cant_nodos--;
    void* dato = nodo->dato;
    free(nodo->clave);
    free(nodo);
    return dato;

}


/* Recibe un arbol y una clave asociada a un valor, se buca en el arbol
 * el valor asociado, si se lo encuentra se lo devuelve. Caso contrario
 * devuelve NULL.
 * Pre: el arbol fue creado.
 */
void *abb_obtener(const abb_t *arbol, const char *clave){
    abb_nodo_t* nodo = abb_nodo_buscar(arbol->raiz,clave,arbol->cmp);
    return (nodo) ? nodo->dato : NULL;
}


/* Recibe un arbol y una clave asociada a un valor, se buca en el arbol
 * el valor asociado, si se lo encuentra se devulve true. Caso contrario
 * devuelve false.
 * Pre: el arbol fue creado.
 */
bool _abb_pertenece(const abb_t *arbol,abb_nodo_t* nodo, const char *clave){
    if(!nodo)
        return false;
    if(arbol->cmp(nodo->clave,clave)==0)
        return nodo->dato;
    if(arbol->cmp(nodo->clave,clave)<0)
        return _abb_pertenece(arbol,nodo->izq,clave);
    if(arbol->cmp(nodo->clave,clave)>0)
        return _abb_pertenece(arbol,nodo->der,clave);
}
bool abb_pertenece(const abb_t *arbol, const char *clave){
    if(!arbol->raiz)
        return false;
    return _abb_pertenece(arbol,arbol->raiz,clave);
}

/* Devuelve la cantida de claves que hay en el arbol.
 * Pre: el arbol fue creado.
 */
size_t abb_cantidad(abb_t *arbol){
    return arbol->cant_nodos;
}

void abb_destruir(abb_t *arbol){

}

/* Itera de manera inorder sobre el arbol aplicandole la funcion visitar
 * a cada clave y dato asociado que se encuentre.
 * Pre: el arbol fue creado.
 */
void _abb_in_order(abb_nodo_t* nodo, bool visitar(const char*, void*, void*), void* extra){
    if(!nodo) return
    _abb_in_order(nodo->izq,visitar,extra);
    visitar(nodo->clave,nodo->dato,extra);
    _abb_in_order(nodo->der,visitar,extra);
}
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra){
    _abb_in_order(arbol->raiz,visitar,extra);
}


/* *****************************************************************
 *                     PRIMITIVAS DEL ITERADOR
 * *****************************************************************/
abb_iter_t* abb_iter_in_crear(const abb_t* arbol){
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if(!iter) return NULL;
    iter->actual = arbol->raiz;
    iter->iterados = 0;
    iter->arbol = arbol;
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t* iter){
    return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter){
    return (iter->nodo) ? iter->nodo->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t* iter){
    return iter->nodo;
}

void abb_iter_in_destruir(abb_iter_t* iter){
    free(iter);
    return;
}
