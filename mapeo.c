#include <stdio.h>
#include <stdlib.h>
#include "mapeo.h"

//Prototipos de funciones globales para el manejo de eliminaci√≥n de elementos del mapeo.
void (*funcion_eliminar_claves)(void *);
void (*funcion_eliminar_valores)(void *);
void funcion_eliminar_entrada(tElemento e);

//Prototipo de la funcion re-hash (Descripcion detallada en su implementacion)
void re_hash(tMapeo m);

//Prototipo de la funcion max (Descripcion detallada en su implementacion)
int max(int n1, int n2);

//Factor de carga definido
const float fc = 0.75;

//IMPLEMENTACION DE LAS OPERACIONES

/**
 Inicializa un mapeo vacio, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves sera computado mediante la funcion fHash.
 A todo efecto, la comparacion de claves se realizara mediante la funcion fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){

    int capacidad_inicial = max(10, ci);

    (*m) = (tMapeo) malloc(sizeof (struct mapeo));

    if (*m == NULL){
        exit (MAP_ERROR_MEMORIA);
    }

    tLista * arraylists = (tLista*) malloc(sizeof(tLista) * capacidad_inicial);

    if (arraylists == NULL){
        exit(MAP_ERROR_MEMORIA);
    }

    for (int i=0; i < capacidad_inicial; i++){
        crear_lista(&arraylists[i]);
    }

    (*m)->cantidad_elementos = 0;
    (*m)->longitud_tabla = capacidad_inicial;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;
    (*m)->tabla_hash = arraylists;
}

/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existia en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
tValor m_insertar(tMapeo m, tClave c, tValor v){
    tLista lista;
    tPosicion pos;
    tPosicion pos_fin;
    int encontre = 0;
    int bucket;
    tValor toRet = NULL;
    tEntrada entry;
    tClave *clave;

    if(m == NULL || c == NULL || v == NULL) {
        exit(MAP_ERROR_MEMORIA);
    }

    bucket = m->hash_code(c) % m->longitud_tabla;
    lista = *(m->tabla_hash+bucket); //obtiene el arreglo en la posici√≥n bucket
    pos = l_primera(lista);
    pos_fin = l_fin(lista);

    while (pos != pos_fin && !encontre) {
        entry = (tEntrada)l_recuperar(lista,pos);
        clave = entry->clave;

        if(m->comparador(clave,c) == 0){//Si es true
            toRet = entry->valor;
            entry->valor = v; //modifico el valor
            encontre = 1;
        }
        else {
            pos = l_siguiente(lista,pos);
        }
    }

    if(!encontre){

        entry = (tEntrada) malloc(sizeof(struct entrada));

        if (entry == NULL){
            exit(MAP_ERROR_MEMORIA);
        }

        entry->clave = c;
        entry->valor = v;
        l_insertar(lista,pos,entry);
        m->cantidad_elementos++;

        if((float)(m->cantidad_elementos+1) / (float)(m->longitud_tabla) >=  fc){ //Si supera el factor de carga definido.
            re_hash(m);
        }
    }

    return toRet;
}

/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    if (c == NULL){
        exit(MAP_ERROR_MEMORIA);
    }

    funcion_eliminar_claves = fEliminarC;
    funcion_eliminar_valores = fEliminarV;

    int seguir = 1;
    int bucket = m->hash_code(c) % m->longitud_tabla;
    tLista tablahash = m->tabla_hash[bucket];
    tEntrada entry;
    tPosicion pos = l_primera(tablahash);
    tPosicion fin = l_fin(tablahash);

    //Se recorre el bucket de la tabla a fin de encontrar la clave con elemento a eliminar.
    while(seguir == 1 && pos != fin){
        entry = (tEntrada) l_recuperar(tablahash, pos);
        if (m->comparador(c, entry->clave) == 0){
            seguir = 0;
            l_eliminar(tablahash, pos, &funcion_eliminar_entrada);
            m->cantidad_elementos--;
        }
        else{
            pos = l_siguiente(tablahash, pos);
        }
    }
}

/**
Procedimiento encargado de eliminar y liberar la memoria ocupada por el mapeo en su totalidad.
@Param *m - tMapeo Mapeo que se va a eliminar completamente.
@Param *fEliminarC Procedimiento encargado de eliminar la clave de cada entrada del mapeo.
@Param *fEliminarV Procedimiento encargado de eliminar el valor de cada entrada del mapeo.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){
    funcion_eliminar_claves = fEliminarC;
    funcion_eliminar_valores = fEliminarV;

    //Se recorren todos los buckets de la tabla, destruyendo asimismo cada lista contenida en ellos.
    for (int i=0; i < (*m)->longitud_tabla; i++){
        l_destruir(&((*m)->tabla_hash[i]), &funcion_eliminar_entrada);
    }

    free(*m);
    free(m->tabla_hash);
    (*m) = NULL;
}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 @return Valor correspondiente a la clave parametrizada, o NULL si Èsta no se encontro.
**/
tValor m_recuperar(tMapeo m, tClave c){
    int bucket = (m->hash_code(c)) % (m->longitud_tabla);
    int encontre = 0;
    tLista mi_lista = m->tabla_hash[bucket];
    tValor toReturn = NULL;
    tPosicion pos_actual;
    tPosicion pos_fin;
    tEntrada entrada;

    pos_actual = l_primera(mi_lista);
    pos_fin = l_fin(mi_lista);
    while(!encontre && pos_actual!=pos_fin){ //Se recorre la lista correspondiente al bucket.
        entrada = (tEntrada) l_recuperar(mi_lista, pos_actual);
        if (m->comparador(c, entrada->clave) == 0){ //Las claves son iguales
            encontre = 1;
            toReturn = entrada->valor;
        }
        else
            pos_actual = l_siguiente(mi_lista, pos_actual);
    }
    return toReturn;
}

//IMPLEMENTACION DE FUNCIONES AUXILIARES

/**
  Procedimiendo encargado de eliminar y liberar el espacio de memoria del elemento (entrada) de la lista.
  @Param: e - tElemento Elemento que se va a eliminar.
**/
void funcion_eliminar_entrada(tElemento e){
    tEntrada entrada = (tEntrada) e;
    funcion_eliminar_valores(entrada->valor);
    funcion_eliminar_claves(entrada->clave);
    free(entrada);
    entrada = NULL;
}

/**
 La utilizamos en reHash para que cuando destruimos la lista, no nos borre las entradas de las mismas.
**/
void noEliminarLasEntradas(){
}


/**
Procedimiento encargado de duplicar el tamaÒo de la tabla hash.
Reinserta a todos los elementos de manera que queden distribuidos uniformemente.
@Param m - tMapeo Mapeo cuya tabla hash sera redimensionada.
**/
void re_hash(tMapeo m) {
    int nuevo_tamanyo = (int) (m->longitud_tabla) * 2;
    int bucket;
    tEntrada entry;
    tPosicion pos, pos_final;
    int long_vieja = (int) m->longitud_tabla;
    tLista lista_actual;
    tLista *lista = (tLista*) malloc((nuevo_tamanyo) * sizeof(tLista));

    //se crean las listas en la tabla hash redimensionada
    for(int i = 0; i < nuevo_tamanyo; i++) {
        crear_lista(&lista[i]);
    }

    //se recorre la tabla hash anterior para copiar su contenido en la nueva tabla.
    for(int i = 0; i < long_vieja; i++) {
        lista_actual = m->tabla_hash[i];
        pos = l_primera(lista_actual);
        pos_final = l_fin(lista_actual);

        //recorrido de las listas de la tablahash[bucket]
        while(pos != pos_final) {
            entry = (tEntrada) l_recuperar(lista_actual, pos);
            bucket = (m->hash_code(entry->clave)) % nuevo_tamanyo;
            l_insertar(lista[bucket], l_primera(lista[bucket]), entry);
            pos = l_siguiente(lista_actual, pos);
        }
        l_destruir(&lista_actual, noEliminarLasEntradas);
    }

    m->longitud_tabla = nuevo_tamanyo;
    free(m->tabla_hash);
    m->tabla_hash = lista;
}

/**
    Funcion que determina el m√°ximo entre dos numeros enteros.
    @Param n1 - int Primer numero entero.
    @Param n2 - int Segundo numero entero.
**/
int max(int n1, int n2){
    int toreturn = 0;

    if (n1 == n2){
        toreturn = n1;
    }
    else if (n1 > n2){
        toreturn = n1;
    }
    else{
        toreturn = n2;
    }
    return toreturn;
}
