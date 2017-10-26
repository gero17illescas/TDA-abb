#include "testing.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h> 

/* *****************************************************************
 *                    PRUEBAS PARA ABB
 * *****************************************************************/

// Realiza pruebas sobre la implementación de la abb del alumno.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_alumno.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en abb.h
// (esto es, las pruebas no pueden acceder a los miembros del struct abb).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.

void abb_vacio(){
    printf("\nVacias\n");
    abb_t* abb = abb_crear(strcmp,free);
    print_test("Guardar cadena nula     ",!abb_guardar(abb,NULL,NULL));
    print_test("Cantidad es 0           ",abb_cantidad(abb)==0);
    abb_destruir(abb);
}
void abb_simple(){
    printf("\nSimples\n");
    abb_t* abb = abb_crear(strcmp,free);
    char* clave = malloc(6);    strcpy(clave,"perro");
    char* dato = malloc(5);     strcpy(dato,"guau");
    print_test("Guardar cadena          ",abb_guardar(abb,clave,dato));
    print_test("Cantidad es 1           ",abb_cantidad(abb)==1);
    print_test("Pertence perro?         ",abb_pertenece(abb,clave));
    print_test("Obtener                 ",strcmp(abb_obtener(abb,clave),dato)==0);
    print_test("Eliminamos              ",strcmp(abb_borrar(abb,clave),dato)==0);
    print_test("Cantidad es 0           ",abb_cantidad(abb)==0);
    free(dato);
    free(clave);
    abb_destruir(abb);
}
void pruebas_abb_alumno(void){
    abb_vacio();
    abb_simple();
    printf("Se termino correctamente el programa\n");
}
int main(){
    pruebas_abb_alumno();
    return 0;
}