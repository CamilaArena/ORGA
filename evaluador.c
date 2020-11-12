#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "string.h"
#include "mapeo.h"

#define ERR_OPEN_FILE -1
#define ERR_INIT_PROGRAM -2
#define ERR_OPTION -3
#define ERR_MEMORY -4

FILE* archivo;
tMapeo map;

void fEliminarC(void *clave);
void fEliminarV(void *valor);

//Funcion de comparacion
/**
Compara los strings pasados por parametro.
@Param *a- string a comparar.
@Param *b- string a comparar.
**/
int funcion_comparacion(void * a, void * b){
  char *string1 = a;
  char *string2 = b;
  int toret = strcmp(string1, string2);
  return toret;
}

//Funcion Hash
/**
 Reinserta a todos los elementos del mapeo de manera que queden distribuidos uniformemente.
 @Param *p- clave a la cual aplicarle la funcion hash.
**/
int funcion_hash(void * p) {
    char * palabra = (char *) p;
    int letraAscii;
    int sumaAscii = 0;

    for(int i=0; i<strlen(palabra); i++) {
        letraAscii = (int) palabra[i];
        sumaAscii += letraAscii + i;
    }
    return sumaAscii;
}


//Funcion eliminar clave.
/**
Procedimiento que elimina y libera la memoria ocupada por la clave parametrizada.
@Param clave- clave a liberar.
**/
void fEliminarC(void *clave) {
    free(clave);
    clave = NULL;
}

//Funcion eliminar valor.
/**
Procedimiento que elimina y libera la memoria ocupada por el valor parametrizado.
@Param valor- valor a liberar.
**/
void fEliminarV(void *valor) {
    free(valor);
    valor = NULL;
}

/**
Procedimiento encargado de guardar todas las palabras del archivo en el mapeo.
@Param m- tMapeo donde guardar las palabras del archivo.
**/
void procesar_archivo(){
    char linea[255];
    char separadores[] = " ,.¿?¡!\n\0";
    int *apariciones;
    char *word;
    char *separados;
    int *valor;

    while(!feof(archivo)) {

        fgets(linea, 100, archivo);
        separados = strtok(linea, separadores);

        while(separados != NULL){
            word = strdup(separados);
            apariciones =  m_recuperar(map, separados);

            if (apariciones == NULL) {
                valor = malloc(sizeof(int));
                if(valor == NULL){
                    exit(ERR_MEMORY);
                }
                *valor = 1;
                m_insertar(map, word, valor);
           }
           else {
               *apariciones = *(apariciones) + 1;
           }
            separados = strtok(NULL, separadores); //de esta manera evitamos un bucle infinito
        }
    }
    fclose(archivo);
}

/**
Procedimiento encargado de finalizar el programa y de liberar la memoria utilizada por los TDA Lista y TDA Mapeo.
**/
int salir(){
    m_destruir(&map, &fEliminarC, &fEliminarV);
    return 0;
}

/**
Funcion encargada de computar la cantidad de apariciones de una determinada palabra en el archivo
@Param *word- es la palabra cuyas apariciones se deben contabilizar.
**/
int cant_apariciones(char * word){
    int cant = 0;
    tValor apariciones = m_recuperar(map, word);
    if (apariciones != NULL)
        cant= *((int*) apariciones);
    return cant;
}

int main(int argc, char * argv[]) {
    argc=2;
    char *filename;
    argv[1]="file.txt";

    if(argc == 2){
        char* word = (char *) malloc(34*sizeof(char));
        if(word == NULL){
            exit(ERR_MEMORY);
        }
        int opcion = 1;
        int apariciones;
        filename = argv[1];
        crear_mapeo(&map, 10, &funcion_hash, &funcion_comparacion);

        //Archivo del programa
        archivo = fopen(filename, "r");
        if (archivo == NULL) {
            printf("Error: no fue posible abrir el archivo\n");
            exit(ERR_OPEN_FILE);
        }

        procesar_archivo(filename);

        printf("Operaciones disponibles: \n");
        printf("1 - Consultar la cantidad de apariciones de una palabra.\n");
        printf("2 - Salir.\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);fflush(stdin);
        printf("\n");

        if(opcion!=1 && opcion!=2){
            printf("Error: la opcion seleccionada es incorrecta");
            exit(ERR_OPTION);
        }

        while(opcion != 2){
            if(opcion == 1){
                printf("Ingrese una palabra: ");
                scanf("%s",(char*) word);fflush(stdin);
                printf("\n");

                apariciones = cant_apariciones(word);

                printf("La cantidad de apariciones de la palabra es %d\n", apariciones);
                printf("Si desea ingresar otra palabra, seleccione 1, sino seleccione 2: ");
                scanf("%d",&opcion);fflush(stdin);
                printf("\n");
            }
            else {
                if(opcion!= 2){
                    printf("Error: la opcion seleccionada es incorrecta");
                    free(word);
                    salir();
                }
            }
        }
    }
    else{
        printf("Error: no se pudo inicializar el programa\n");
        exit(ERR_INIT_PROGRAM);
    }

    return 0;
}
