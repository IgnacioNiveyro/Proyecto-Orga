#include <stdio.h>
#include <stdlib.h>

#include "lista.h"
#include "mapeo.h"


void reHash(tMapeo m);
void (*fEliminarClave)(void *);
void (*fEliminarValor)(void *);

/**
  Compara dos números enteros y retorna el mayor. Si son iguales, retorna el segundo.
**/
int MAX(int a, int b){

    int maximo = 0;

    if(a > b)
        maximo = a;
    else
        maximo = b;

    return maximo;
}

/**
   Elimina la entrada y los campos.
**/
void fEliminarEntradaYCampos(tElemento elemento){

    tEntrada entrada = (tEntrada) elemento;

    fEliminarClave(entrada->clave);
    fEliminarValor(entrada->valor);

    entrada->clave = NULL;
    entrada->valor = NULL;

    free (entrada);
    entrada = NULL;
}

/**
   Elimina la entrada.
**/
void fEliminarEntrada(tElemento elemento){

    tEntrada entrada = (tEntrada) elemento;
    free(entrada);
}

/**
 No elimina nada.
**/
void noElimino(){

}

/**
 Inicializa un mapeo vacío, con capacidad inicial igual al MAX(10, CI).
 Una referencia al mapeo creado es referenciada en *M.
 A todo efecto, el valor hash para las claves será computado mediante la función fHash.
 A todo efecto, la comparación de claves se realizará mediante la función fComparacion.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void crear_mapeo(tMapeo * m, int ci, int (*fHash)(void *), int (*fComparacion)(void *, void *)){

    int tamanio = (MAX(10,ci));
    (*m) = (tMapeo) malloc(sizeof (struct mapeo));

    if ((*m) == NULL)
        exit (MAP_ERROR_MEMORIA);

    (*m)->cantidad_elementos = 0;
    (*m)->longitud_tabla = tamanio;
    (*m)->hash_code = fHash;
    (*m)->comparador = fComparacion;

    tLista * arregloBuckets = (tLista*) malloc(sizeof(tLista) * tamanio);
    if(arregloBuckets == NULL)
        exit(MAP_ERROR_MEMORIA);

    (*m)->tabla_hash = arregloBuckets;

    for (int i=0; i < tamanio; i++){
        crear_lista(&arregloBuckets[i]);
    }
}

/**
  Redimensiona el arreglo de buckets.
**/
void reHash(tMapeo m){

    tEntrada entrada;
    tLista listaBucket;
    tPosicion pos_actual, pos_final;
    tClave clave;
    int bucketNuevo, i, j = 0;
    int nuevaLongitud =  m->longitud_tabla*2;

    tLista * nuevoArregloBucket = (tLista*) malloc(sizeof(tLista) * nuevaLongitud);

    for(i = 0; i<nuevaLongitud; i++){

        crear_lista(&nuevoArregloBucket[i]);
    }

    for(j = 0; j<m->longitud_tabla; j++){

        listaBucket = m->tabla_hash[j];

        pos_actual = l_primera(listaBucket);
        pos_final = l_fin(listaBucket);

        while(pos_actual != pos_final){

            entrada = l_recuperar(listaBucket, pos_actual);
            clave = entrada->clave;
            bucketNuevo =(m->hash_code(clave) % nuevaLongitud);
            l_insertar(nuevoArregloBucket[bucketNuevo], l_primera(nuevoArregloBucket[bucketNuevo]), entrada);
            pos_actual = l_siguiente(listaBucket, pos_actual);
        }

        l_destruir(&listaBucket, noElimino);
    }

    free(m->tabla_hash);
    m->longitud_tabla = nuevaLongitud;
    m->tabla_hash = nuevoArregloBucket;
}


/**
 Inserta una entrada con clave C y valor V, en M.
 Si una entrada con clave C y valor Vi ya existe en M, modifica Vi por V.
 Retorna NULL si la clave C no existía en M, o Vi en caso contrario.
 Finaliza indicando MAP_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
tValor m_insertar(tMapeo m, tClave c, tValor v){

    tValor valorRetorno = NULL;
    tPosicion pos_actual, pos_final;
    tEntrada entrada;
    int encontre = 0;

    int bucket =((m->hash_code(c)) % (m->longitud_tabla));
    tLista listaBucket = m->tabla_hash[bucket];
    pos_actual = l_primera(listaBucket);
    pos_final = l_fin(listaBucket);

    while(!encontre && pos_actual != pos_final){

        entrada = (tEntrada) l_recuperar(listaBucket, pos_actual);

        if(m->comparador(entrada->clave, c) == 0){

                valorRetorno = entrada->valor;
                entrada->valor = v;
                encontre = 1;
        }
        else
            pos_actual = l_siguiente(listaBucket, pos_actual);
    }


    if(!encontre){

        tEntrada  entradaNueva = (tEntrada) malloc(sizeof(struct entrada));

        if(entradaNueva==NULL)
            exit(MAP_ERROR_MEMORIA);

        (entradaNueva)->clave = c;
        (entradaNueva)->valor = v;
        l_insertar(listaBucket, pos_actual, entradaNueva);

        (m->cantidad_elementos)=(m->cantidad_elementos)+1;

        if((m->cantidad_elementos / m->longitud_tabla) >= 0.75)
            reHash(m);
    }

    return valorRetorno;
}

/**
 Elimina la entrada con clave C en M, si esta existe.
 La clave y el valor de la entrada son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_eliminar(tMapeo m, tClave c, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

    tEntrada entrada;
    tPosicion pos_actual;
    tPosicion pos_final;
    fEliminarClave = fEliminarC;
    fEliminarValor = fEliminarV;

    int encontre = 0;
    int bucket =(m->hash_code(c) % m->longitud_tabla);
    tLista listaBucket = m->tabla_hash[bucket];

    pos_actual =l_primera(listaBucket);
    pos_final = l_fin(listaBucket);


    while((pos_actual != pos_final) && (encontre == 0)){

        entrada = l_recuperar(listaBucket, pos_actual);

        if(m->comparador(c, entrada->clave) == 0){

            encontre = 1;
            fEliminarClave(entrada-> clave);
            fEliminarValor(entrada->valor);
            entrada->clave = NULL;
            entrada->valor = NULL;
            l_eliminar(listaBucket, pos_actual, &fEliminarEntrada);
            (m->cantidad_elementos) = (m->cantidad_elementos)-1;

        }
        else
            pos_actual= l_siguiente(listaBucket, pos_actual);
    }

}

/**
 Destruye el mapeo M, elimininando cada una de sus entradas.
 Las claves y valores almacenados en las entradas son eliminados mediante las funciones fEliminarC y fEliminarV.
**/
void m_destruir(tMapeo * m, void (*fEliminarC)(void *), void (*fEliminarV)(void *)){

    fEliminarClave = fEliminarC;
    fEliminarValor = fEliminarV;
    int tamanioMax = (*m)->longitud_tabla;
    int i;

    for(i = 0; i < tamanioMax ;i++){

        l_destruir(&((*m)->tabla_hash[i]), &fEliminarEntradaYCampos);
    }
    //Invalida campos simples del mapeo
    (*m)->cantidad_elementos=0;
    (*m)->comparador = NULL;
    (*m)->longitud_tabla=0;

    //Libera el espacio de los campos del tipo struct dentro del mapeo
    free((*m)->tabla_hash);
    (*m)->tabla_hash = NULL;

    //Libera espacio del struct mapeo
    free(*m);

    //Modifica la variabla del cliente
    (*m)=NULL;
}

/**
 Recupera el valor correspondiente a la entrada con clave C en M, si esta existe.
 Retorna el valor correspondiente, o NULL en caso contrario.
**/
tValor m_recuperar(tMapeo m, tClave c){

    tPosicion pos_actual;
    tPosicion pos_final;
    tEntrada entrada;
    tValor toReturn = NULL;
    int encontre = 0;

    int bucket = m->hash_code(c) % m->longitud_tabla;
    tLista listaBucket = m->tabla_hash[bucket];
    pos_actual = l_primera(listaBucket);
    pos_final = l_fin(listaBucket);

    while(!encontre && pos_actual != pos_final){

        entrada = (tEntrada) l_recuperar(listaBucket, pos_actual);

        if(m->comparador(c, entrada->clave) == 0){

            encontre = 1;
            toReturn= entrada->valor;
        }
        else
            pos_actual= l_siguiente(listaBucket, pos_actual);
    }
    return toReturn;
}
