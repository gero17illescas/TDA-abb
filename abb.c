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

abb_nodo_t* abb_nodo_buscar(abb_nodo_t*nodo,char* clave,abb_comparar_clave_t cmp){
    if(!nodo)
        return NULL;
    if(cmp(nodo->clave,clave)==0)
        return nodo;
    if(cmp(nodo->clave,clave)<0)
        return abb_nodo_buscar(nodo->izq,clave,cmp);
    if(cmp(nodo->clave,clave)>0)
        return abb_nodo_buscar(nodo->der,clave,cmp);
}


abb_nodo_t* _abb_nodo_buscar_sucesor(abb_nodo_t*nodo,abb_nodo_t*sucesor,char* clave,abb_comparar_clave_t cmp){
    if(!nodo)
        return NULL;
    if(cmp(nodo->clave,clave)==0)
        return sucesor;
    if(cmp(nodo->clave,clave)<0)
        return _abb_nodo_buscar_sucesor(nodo->izq,nodo,clave,cmp);
    if(cmp(nodo->clave,clave)>0)
        return _abb_nodo_buscar_sucesor(nodo->der,nodo,clave,cmp);
}

abb_nodo_t* abb_nodo_buscar_sucesor(abb_t* arbol,char* clave){
    return _abb_nodo_buscar_sucesor(arbol->raiz,NULL,clave,arbol->cmp);
}
abb_nodo_t* abb_nodo_buscar_minimo(abb_nodo_t*nodo){
    if(nodo->izq)
        return abb_nodo_buscar_minimo(nodo->izq);
    if(nodo->der)
        return abb_nodo_buscar_minimo(nodo->der);
    return nodo;
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
    arbol->cant_nodos = 0;
    return arbol;
}

/* Reciben un valor y una clave asociada al valor y lo guarda en el arbol.
 * Devuleve un boolenao si la operacion fue definida.
 * Pre: el arbol fue previamente creado.
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    if(!arbol->raiz){
        arbol->raiz = crear_nodo_abb(clave,dato);
        arbol->cant_nodos++
        if(!arbol->raiz) return false;
    }

    if(arbol->cmp(arbol->raiz,clave)<0)
        return abb_guardar(arbol->izq,clave,dato);

    if(arbol->cmp(arbol->raiz,clave)>0)
        return abb_guardar(arbol->der,clave,dato);

    if(arbol->cmp(arbol->raiz,clave)==0){
        arbol->destruir_dato(arbol->raiz->dato);
        arbol->raiz->dato = dato;
        return true;
    }
}

/* Recibe una clave y se encarga de eliminar el dato asociado y lo 
 * devuelve. Si la clave no pertenece a ningun dato del arbol, o este
 * esta vacio se devuelve NULL.
 * Pre: el arbol fue creado.
 */
void *abb_borrar(abb_t *arbol, const char *clave){
    abb_nodo_t* nodo = abb_nodo_buscar(arbol->raiz,clave,arbol->cmp);
    abb_nodo_t* sucesor = abb_nodo_buscar_sucesor(arbol,clave);
    if(!nodo) return NULL;
    if(sucesor){
        if(!nodo->izq && !nodo->der){
            if(sucesor->izq == nodo)
                sucesor->izq = NULL;
            else
                sucesor->der = NULL;
        }
        if(nodo->izq && !nodo->der){
            if(sucesor->izq == nodo)
                sucesor->izq = nodo->izq;
            if(sucesor->der == nodo)
                sucesor->der = nodo->izq;
        }
        if(!nodo->izq && nodo->der){
            if(sucesor->izq == nodo)
                sucesor->izq = nodo->der;
            if(sucesor->der == nodo)
                sucesor->der = nodo->der;
        }
        if(nodo->izq && nodo->der){
            abb_nodo_t* minimo_der = abb_nodo_buscar_minimo(nodo->der,clave);
            if(sucesor->izq == nodo)
                sucesor->izq = minimo_der;
            if(sucesor->der == nodo)
                sucesor->der = minimo_der;
        }
    }
    else{
        arbol->raiz = abb_nodo_buscar_minimo(nodo->der,clave);
    }
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
bool abb_pertenece(const abb_t *arbol, const char *clave){
    if(!arbol->raiz)
        return NULL;
    if(arbol->raiz==clave)
        return arbol->raiz->dato;
    if(arbol->raiz<clave)
        return abb_pertenece(arbol->izq,clave);
    if(arbol->raiz>clave)
        return abb_pertenece(arbol->der,clave);
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
    visitar(nodo->clave,nodo->dato,extra);
    _abb_in_order(nodo->izq,visitar,extra);
    _abb_in_order(nodo->der,visitar,extra);
}
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra){
    _abb_in_order(arbol->raiz,visitar,extra);
}


/* *****************************************************************
 *                     PRIMITIVAS DEL ITERADOR
 * *****************************************************************/
abb_iter_t* abb_iter_in_crear(const abb_t* arbol);

bool abb_iter_in_avanzar(abb_iter_t* iter);

const char* abb_iter_in_ver_actual(const abb_iter_t* iter);

bool abb_iter_in_al_final(const abb_iter_t* iter);

void abb_iter_in_destruir(abb_iter_t* iter);
