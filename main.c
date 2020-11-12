//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "lista.h"
//#include "mapeo.h"
//
//void mostrarLista(tLista l);
//
//void fEliminar(tElemento e){
//    free(e);
//    e = NULL;
//
//}
//
//#define A 54059 /* a prime */
//#define B 76963 /* another prime */
//#define C 86969 /* yet another prime */
//#define FIRSTH 37 /* also prime */
//int funcion_hash(void *sa){
//        char *s = sa;
//      unsigned h = FIRSTH;
//      while (*s) {
//        h = (h * A) ^ (s[0] * B);
//        s++;
//      }
//      return h; // or return h % C;
//}
//
//int funcion_comparacion(void* a, void* b){
//    char *string_a=a;
//    char *string_b=b;
//    return strcmp(string_a,string_b);
//}
//
//void mostrarLista(tLista l){
//    tEntrada entry;
//    tPosicion pos;
//    if(l==NULL)
//        printf("Lista vacia");
//    else{
//        printf("Lista = <");
//        pos=l_primera(l);
//        for(int i=0; i<l_longitud(l); i++){
//            entry = (tEntrada)l_recuperar(l, pos);
//
//            printf(" (%d, %d) ", entry->clave, entry->valor);
//            pos = l_siguiente(l,pos);
//        }
//        printf(">");
//        printf("\n");
//    }
//}
//
//void mostrarMapeo(tMapeo m){
//    tLista *tablahash = m->tabla_hash;
//
//    for (int i=0; i < m->longitud_tabla; i++){
//        mostrarLista(tablahash[i]);
//        printf("\n");
//    }
//}
//
//void fEliminarC(tClave clave)
//{
//    free(clave);
//    clave = NULL;
//}
//
///**
//    Elimina un valor parametrizado.
//**/
//void fEliminarV(tValor valor)
//{
//    free(valor);
//    valor = NULL;
//}
//
//int main(){
//
////IDEA: MAPEO DE String A INT
//    tMapeo mi_mapeo;
//
//    crear_mapeo(&mi_mapeo, 10, &(funcion_hash), &(funcion_comparacion));
//
//    char *s1 = "nicolas";
//    tValor n = 3;
//    printf("%d", s1);
//    m_insertar(mi_mapeo, s1, n);
//
//    char *s2 = "jswjdj";
//    tValor n2 = 1;
//
//    m_insertar(mi_mapeo, s2, n2);
//
//
//    mostrarMapeo(mi_mapeo);
//
///*
//    tLista lista;
//    crear_lista(&lista);
//    printf("%d", lista);
//    printf("Lista creada con éxito. \n");
//    printf("La longitud es: %d\n", l_longitud(lista));
//    printf("Ingrese un número entero: ");
//    int * num = (int*) malloc(sizeof(int));
//    scanf("%i", &num);
//
//    l_insertar(lista, l_fin(lista), num);
//    printf("La longitud de la lista es: %d\n", l_longitud(lista));
//    mostrarLista(lista);
//
//    printf("Ingrese un número entero: ");
//    scanf("%i", &num);
//    l_insertar(lista, l_fin(lista), num);
//    printf("La longitud de la lista es: %d\n", l_longitud(lista));
//    mostrarLista(lista);
//
//    printf("Ingrese un número entero: ");
//    scanf("%i", &num);
//    l_insertar(lista, l_fin(lista), num);
//    printf("La longitud de la lista es: %d\n", l_longitud(lista));
//    mostrarLista(lista);
//
//    printf("Primer elemento: ");
//    printf("%d\n", l_recuperar(lista, l_primera(lista)));
//
//    printf("Segundo elemento: ");
//    int n = (int *) l_recuperar(lista, l_siguiente(lista, l_primera(lista)));
//    printf("%d\n", n);
//
//    printf("Ultimo elemento: ");
//    int n2 = (int *) l_recuperar(lista, l_siguiente(lista, l_siguiente(lista, l_primera(lista))));
//    printf("%d\n", n2);
//
//    printf("Anterior al ultimo (o sea el segundo): ");
//    int n4 = (int *) l_recuperar(lista, l_anterior(lista, l_ultima(lista)));
//    printf("%d\n", n4);
//
//    printf("Anterior al segundo (o sea el primero): ");
//    int n5 = (int *) l_recuperar(lista, l_anterior(lista, l_anterior(lista, l_ultima(lista))));
//    printf("%d\n", n5);
//
//    printf("Eliminacion del ultimo: \n");
//    l_eliminar(lista, l_ultima(lista), &fEliminar);
//    mostrarLista(lista);
//
//    printf("Eliminacion del primero: \n");
//    l_eliminar(lista, l_primera(lista), &fEliminar);
//    mostrarLista(lista);
//    /*
//    printf("Elemento en la posicion fin: ");
//    int n3 = (int *) l_recuperar(lista, l_fin(lista));
//    printf("%d\n", n3); //da error macro numero 3
//    */
//}
