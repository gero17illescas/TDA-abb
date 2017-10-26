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
    print_test("Borrar clave es NULL    ",!abb_borrar(abb,NULL));
    abb_destruir(abb);
}
void abb_simple(){
    printf("\nSimples\n");
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1 = "guau";

    print_test("Guardar clave1          ",abb_guardar(abb,clave1,valor1));
    print_test("Cantidad es 1           ",abb_cantidad(abb)==1);
    print_test("Pertence clave1?        ",abb_pertenece(abb,clave1));
    print_test("Obtener                 ",strcmp(abb_obtener(abb,clave1),valor1)==0);
    print_test("Eliminamos              ",strcmp(abb_borrar(abb,clave1),valor1)==0);
    print_test("Cantidad es 0           ",abb_cantidad(abb)==0);
    abb_destruir(abb);
}
void abb_multiples(){
    printf("\nMultiples\n");
    abb_t* abb = abb_crear(strcmp,free);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";

    print_test("Guardar clave1          ",abb_guardar(abb,clave1,valor1));
    print_test("Cantidad es 1           ",abb_cantidad(abb)==1);
    print_test("Pertence clave1?        ",abb_pertenece(abb,clave1));
    print_test("Obtener                 ",strcmp(abb_obtener(abb,clave1),valor1)==0);

    print_test("Guardar clave2          ",abb_guardar(abb,clave2,valor2));
    print_test("Cantidad es 2           ",abb_cantidad(abb)==2);
    print_test("Pertence clave2?        ",abb_pertenece(abb,clave2));
    print_test("Obtener                 ",strcmp(abb_obtener(abb,clave2),valor2)==0);

    print_test("Guardar clave3          ",abb_guardar(abb,clave3,valor3));
    print_test("Cantidad es 3           ",abb_cantidad(abb)==3);
    print_test("Pertence clave3?        ",abb_pertenece(abb,clave3));
    print_test("Obtener                 ",strcmp(abb_obtener(abb,clave3),valor3)==0);

    print_test("Eliminamos clave1       ",strcmp(abb_borrar(abb,clave1),valor1)==0);
    print_test("Eliminamos clave2       ",strcmp(abb_borrar(abb,clave2),valor2)==0);
    print_test("Eliminamos clave3       ",strcmp(abb_borrar(abb,clave3),valor3)==0);
    print_test("Cantidad es 0           ",abb_cantidad(abb)==0);

}
void pruebas_abb_alumno(void){
    abb_vacio();
    abb_simple();
    abb_multiples();
    printf("Se termino correctamente el programa\n");
}
int main(){
    pruebas_abb_alumno();
    return 0;
}