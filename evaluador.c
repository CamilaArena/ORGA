#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "mapeo.h"

#define ERR_OPEN_FILE -1 //macro de error al abrir un archivo.
#define ERR_INIT_PROGRAM -2 //macro de error al iniciar la aplicacion.
#define ERR_OPTION -3 //macro de error al detectar una opcion invalida ingresada.
#define ERR_MEMORY -4 //macro de error de reservacion de memoria.

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
Procedimiento encargado de guardar todas las palabras del archivo en el mapeo global.
**/
void procesar_archivo(){
    char linea[255];
    int *apariciones;
    char separadores[] = " ,.¿?¡!\n\0";
    char *word;
    char *separados;
    int *valor;

    while(!feof(archivo)) {
        fgets(linea, 100, archivo);
        separados = strtok(linea, separadores);

        while(separados != NULL){
            word = strdup(separados);
            if (word == NULL){
                printf("Error de memoria.");
                exit(ERR_MEMORY);
            }

            apariciones =  m_recuperar(map, separados);

            //comienza a insertar las palabras separadas en el mapeo dependiendo de si ya se encuentran o no
            if (apariciones == NULL) {
                valor = malloc(sizeof(int));
                if(valor == NULL){
                    printf("Error de memoria.");
                    exit(ERR_MEMORY);
                }

                *valor = 1;
                m_insertar(map, word, valor);
           }
           else {
                free(word);
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
    printf("Cerrando programa...\n");
    printf("Gracias por utilizar nuestra aplicacion.\n");

    return 0;
}

/**
Funcion encargada de computar la cantidad de apariciones de una determinada palabra en el archivo
@Param *word- palabra cuyas apariciones se deben contabilizar.
**/
int cant_apariciones(char * word){
    int cant = 0;
    tValor apariciones = m_recuperar(map, word);
    if (apariciones != NULL)
        cant= *((int*) apariciones);
    return cant;
}

//Programa principal
/**
    Procedimiento main encargado de procesar el archivo para ofrecer la operacion de
    consultar la cantidad de apariciones por palabra. Se debe ejecutar por consola/terminal.
    @Param argc - Cantidad de parametros enviados a traves de la consola.
    @Param argv - Arreglo con los parametros enviados a traves de la consola.
**/
int main(int argc, char * argv[]) {
    char *filename;
    int apariciones;
    int opcion;
    char* word;

    if(argc == 2){
        word = (char *) malloc(34*sizeof(char));
        if(word == NULL){
            printf("Error de memoria.");
            exit(ERR_MEMORY);
        }

        filename = argv[1];
        crear_mapeo(&map, 10, &funcion_hash, &funcion_comparacion);

        //Archivo del programa
        archivo = fopen(filename, "r");
        if (archivo == NULL) {
            printf("Error: no fue posible abrir el archivo.\n");
            exit(ERR_OPEN_FILE);
        }

        procesar_archivo(filename);

        //Implementacion del menu
        printf("Operaciones disponibles: \n");
        printf("1 - Consultar la cantidad de apariciones de una palabra.\n");
        printf("2 - Salir.\n");
        printf("Ingrese una opcion: ");
        opcion = -1;
        scanf("%d", &opcion);
        fflush(stdin);
        printf("\n");

        while (opcion == 1){
            printf("Ingrese una palabra: ");
            scanf("%s",(char*) word);
            fflush(stdin);
            printf("\n");

            apariciones = cant_apariciones(word);

            printf("La cantidad de apariciones de la palabra '%s' es %d\n", word, apariciones);
            printf("Si desea consultar otra palabra, ingrese 1, sino ingrese 2: ");
            opcion = -1; //para que detecte error de opcion si se ingresa una cadena de caracteres como opcion
            scanf("%d", &opcion);
            fflush(stdin);
            printf("\n");
        }

        if (opcion == 2){
            free(word);
            salir();
        }
        else {
            printf("Error: la opcion ingresada es incorrecta.\n");
            free(word);
            salir();
            exit(ERR_OPTION);
        }
    }
    else{
        printf("Error: no se pudo inicializar el programa.\n");
        exit(ERR_INIT_PROGRAM);
    }

    return 0;
}
