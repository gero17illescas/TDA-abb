#include "testing.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>


int randomizer(){
    return rand()%1000000000;
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
    abb_t* abb = abb_crear(strcmp,NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    char *valor4 = "pio"; 
    char *valor5 = NULL;

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

    print_test("Guardar clave3          ",abb_guardar(abb,clave3,valor4));
    print_test("Cantidad es 3           ",abb_cantidad(abb)==3);
    print_test("Pertence clave3?        ",abb_pertenece(abb,clave3));
    print_test("Obtener(se reemplazo)   ",strcmp(abb_obtener(abb,clave3),valor4)==0);

    print_test("Guardar clave3          ",abb_guardar(abb,clave3,valor5));
    print_test("Cantidad es 3           ",abb_cantidad(abb)==3);
    print_test("Pertence clave3(NULL)?  ",abb_pertenece(abb,clave3));
    print_test("Obtener es NULL         ",abb_obtener(abb,clave3)==NULL);

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
    printf("Inicio pruebas de volumen\n");
    abb_t* arbol = abb_crear(strcmp, NULL);
    size_t largo = 5000;
    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);
    unsigned* valores[largo];

    bool ok = true;
    for (int i = 0; i < largo; i++) {
        int rand = randomizer();
        valores[i] = malloc(sizeof(int));
        sprintf(claves[i], "%08d", rand);
        *valores[i] = rand;
        ok = abb_guardar(arbol, claves[i], valores[i]);
    }

    print_test("Prueba abb almacenar muchos elementos: ", ok);
    print_test("Cantidad de elementos es correcta: ", abb_cantidad(arbol) == largo);

    for(size_t i = 0; i < largo; i++){
        ok = abb_pertenece(arbol, claves[i]);
        if(!ok) break;
        ok = abb_obtener(arbol, claves[i]) == valores[i];
        if(!ok) break;
    }

    print_test("Prueba pertenece y obtener muchos elementos: ", ok);
    print_test("Prueba cantidad de elementos es correcta: ", abb_cantidad(arbol) == largo);

    for(size_t i = 0; i < largo; i++){
        ok = abb_borrar(arbol, claves[i]) == valores[i];
        if(!ok) break;
    }

    print_test("Prueba abb borrar muchos elementos,", ok);
    print_test("Prueba cant de elementos es 0 ", abb_cantidad(arbol) == 0);

    abb_destruir(arbol);
    abb_t* abb = abb_crear(strcmp, free);

    /* Inserta 'largo' parejas en el abb */
    ok = true;
    for (size_t i = 0; i < largo; i++) {
        ok = abb_guardar(abb, claves[i], valores[i]);
        if (!ok) break;
    }

    free(claves);
    /* Destruye el abb - debería liberar los enteros */
    abb_destruir(abb);
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


void pruebas_abb_iterar_volumen(){
    abb_t* abb = abb_crear(strcmp, NULL);
<<<<<<< HEAD

    size_t largo = 5000;
    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];

=======

    size_t largo = 5000;
    const size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);

    size_t valores[largo];

>>>>>>> 615af7b0b5d11866599d4ef33ffeaaf25a780c61

    bool ok = true;
    for (int i = 0; i < largo; i++) {
        int rand = randomizer();
        sprintf(claves[i], "%08d", rand);
        valores[i] = i;
        ok = abb_guardar(abb, claves[i], &valores[i]);
        if (!ok) break;
    }

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
    pruebas_iter_elementos();
    pruebas_abb_iterar_volumen();
    printf("Se termino correctamente el programa\n");
}
<<<<<<< HEAD
=======
int main(){
    pruebas_abb_alumno();
    return 0;
}
>>>>>>> 615af7b0b5d11866599d4ef33ffeaaf25a780c61
