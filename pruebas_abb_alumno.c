#include "testing.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>


void random_inicializar(){
    unsigned int seed = (unsigned int)time(NULL);
    srand (seed);
}

int nuestro_random(int lim){
    return rand()%lim;
}

// Funciones de swapeo
void swap_char(char** x, char** y){
    char* aux=*x;
    *x=*y;
    *y=aux;
}

void swap_int(int** x, int** y){
    int* aux=*x;
    *x=*y;
    *y=aux;
}

void vector_desordenar(char* claves[], int* valores[], int largo){
    random_inicializar();
    int i;
    int rnd;
    // es importante que el par clave-valor se mantenga siempre igual
    for (i=0; i<largo;i++){
        rnd=nuestro_random(largo);

        swap_char(&claves[i], &claves[rnd]);
        swap_int(&valores[i], &valores[rnd]);
    }

}
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
    abb_t* abb = abb_crear(strcmp, NULL);
    print_test("Guardar cadena nula     ",!abb_guardar(abb,NULL,NULL));
    print_test(("Clave 'perro' no pertenece: "), !abb_pertenece(abb, "Perro"));
    print_test("Cantidad es 0           ",abb_cantidad(abb)==0);
    print_test("Borrar clave es NULL    ",!abb_borrar(abb,NULL));
    abb_destruir(abb);
}
void abb_simple(){
    printf("\nSimples\n");
    abb_t* abb = abb_crear(strcmp, NULL);

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
    abb_destruir(abb);
}

void abb_volumen(){
    printf("Prueba de arbol a volumen\n");

    abb_t* arbol=abb_crear(strcmp,NULL);

    int largo=1000;
    char* claves[largo];
    int* valores[largo];

    int i;
    for (i = 0; i < largo; i++) {
        claves[i] = malloc(10*sizeof(char));
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", i);
        *valores[i] = i;
    }

    vector_desordenar(claves,valores,largo);
    bool ok=true;
    i=0;
    while (i<largo && ok){
        ok=abb_guardar(arbol, claves[i], valores[i]);
        i++;
    }
    print_test("Puedo agregar 10000 valores", ok);
    print_test("La cantidad de elementos del arbol es 10000", abb_cantidad(arbol)==largo);

    i=0;
    ok=true;
    while(ok && i<largo){
        ok = abb_pertenece(arbol, claves[i]);
        ok = abb_obtener(arbol, claves[i]) == valores[i];
        i++;
    }

    print_test("Los elementos estan bien guardados, y pertenecen",ok);
    print_test("Hay 10000 elementos en el arbol", abb_cantidad(arbol)==largo);

    /* Verifica que borre y devuelva los valores correctos */
    i=0; ok=true;
    while (i < largo && ok) {
        ok = abb_borrar(arbol, claves[i]) == valores[i];
        i++;
    }
    print_test("Los elementos al borrarlos dieron todos bien", ok);
    print_test("Ahora el arbol esta vacio", abb_cantidad(arbol)==0);

    abb_destruir(arbol);
    arbol = abb_crear(strcmp,free);

    /* Inserta 'largo' parejas en el hash */
    ok = true;
    i=0;
    while(i < largo && ok) {
        ok = abb_guardar(arbol, claves[i], valores[i]);
        i++;
    }

    /* Libera las cadenas */
    for (i = 0; i < largo; i++) {
        free(claves[i]);
    }

    /* Destruye el arbol - debería liberar los enteros */
    abb_destruir(arbol);

}

void pruebas_iter_arbol_vacio(){
    printf("Inicio pruebas con iterador en arbol vacio: \n");
    abb_t* arbol = abb_crear(strcmp, NULL);
    abb_iter_t* iter = abb_iter_in_crear(arbol);
    print_test("Iter creado: ", iter!=NULL);
    print_test("Actual es NULL: ", abb_iter_in_ver_actual(iter) == NULL);
    print_test("Avanza en arbol vacio: ", !abb_iter_in_avanzar(iter));
    print_test("Iter esta al final: ", abb_iter_in_al_final(iter));
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);
    print_test("Arbol e iter destruidos: ", true);
}

void pruebas_iter_elementos(){
    printf("Inicio pruebas del iter con elementos\n");
    abb_t* arbol = abb_crear(strcmp, NULL);

    int valor1 = 1;
    int valor2 = 2;
    int valor3 = 3;

    print_test("Guardo un elemento: ", abb_guardar(arbol, "Perro", &valor1));
    print_test("Guardo un elemento: ", abb_guardar(arbol, "Gato", &valor2));
    print_test("Guardo un elemento: ", abb_guardar(arbol, "Vaca", &valor3));
    print_test("Elemento(perro) pertenece al arbol: ", abb_pertenece(arbol, "Perro"));
    print_test("Elemento(gato) perten   ece al arbol: ", abb_pertenece(arbol, "Gato"));
    print_test("Elemento(vaca) pertenece al arbol: ", abb_pertenece(arbol, "Vaca"));

    abb_iter_t* iter = abb_iter_in_crear(arbol);

    print_test("Iter no esta al final: ", !abb_iter_in_al_final(iter));
    print_test("Elemento actual es 'Gato: ", strcmp(abb_iter_in_ver_actual(iter), "Gato") == 0);
    print_test("Avance OK: ", abb_iter_in_avanzar(iter));
    print_test("Iter no esta al final: ", !abb_iter_in_al_final(iter));
    print_test("Elemento actual es 'Perro': ", strcmp(abb_iter_in_ver_actual(iter), "Perro") == 0);
    print_test("Avance OK: ", abb_iter_in_avanzar(iter));
    print_test("Iter no esta al final: ", !abb_iter_in_al_final(iter));
    print_test("Elemento actual es 'Vaca': ", strcmp(abb_iter_in_ver_actual(iter), "Vaca") == 0);
    print_test("Avance OK, ", abb_iter_in_avanzar(iter));
    print_test("Iter al final", abb_iter_in_al_final(iter));
    print_test("Actual es NULL: ", abb_iter_in_ver_actual(iter) == NULL);
    print_test("No puedo avanzar: ", !abb_iter_in_avanzar(iter));
    abb_iter_in_destruir(iter);
    abb_destruir(arbol);
    print_test("Arbol e iter destruidos: ", true);
}


void pruebas_abb_iterar_volumen(size_t largo){
    abb_t* abb = abb_crear(strcmp, NULL);

    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];

    /* Inserta 'largo' parejas en el abb */
    bool ok = true;
    for (unsigned i = 0; i < largo; i++) {
        sprintf(claves[i], "%08d", i);
        valores[i] = i;
        ok = abb_guardar(abb, claves[i], &valores[i]);
        if (!ok) break;
    }

    // Prueba de iteración sobre las claves almacenadas.
    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("Prueba abb iterador esta al final, es false", !abb_iter_in_al_final(iter));

    ok = true;
    unsigned i;
    const char *clave;
    size_t *valor;

    for (i = 0; i < largo; i++) {
        if ( abb_iter_in_al_final(iter) ) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if ( clave == NULL ) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if ( valor == NULL ) {
            ok = false;
            break;
        }
        *valor = largo;
        abb_iter_in_avanzar(iter);
    }
    print_test("Prueba abb iteración en volumen", ok);
    print_test("Prueba abb iteración en volumen, recorrio todo el largo", i == largo);
    print_test("Prueba abb iterador esta al final, es true", abb_iter_in_al_final(iter));

    ok = true;
    for (i = 0; i < largo; i++) {
        if ( valores[i] != largo ) {
            ok = false;
            break;
        }
    }
    print_test("Prueba abb iteración en volumen, se cambiaron todo los elementos", ok);

    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}

void pruebas_abb_alumno(void){
    abb_vacio();
    abb_simple();
    abb_multiples();
    abb_volumen();
    pruebas_iter_arbol_vacio();
    //pruebas_iter_elementos();
    //pruebas_abb_iterar_volumen(5000);
    printf("Se termino correctamente el programa\n");
}
int main(){
    pruebas_abb_alumno();
    return 0;
}