#include <stdio.h>
#include <stdlib.h>

#include "lista.h"

/**
 Inicializa una lista vacía.
 Una referencia a la lista creada es referenciada en *L.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/

void crear_lista(tLista * l){
    (*l)=(tLista) malloc(sizeof(struct celda));
    if((*l)==NULL)
        exit(LST_ERROR_MEMORIA);
    (*l)->elemento=NULL;
    (*l)->siguiente=NULL;
}

/**
 Inserta el elemento E, en la posición P, en L.
 Con L = A,B,C,D y la posición P direccionando C, luego:
 L' = A,B,E,C,D.
 Finaliza indicando LST_ERROR_MEMORIA si no es posible reservar memoria correspondientemente.
**/
void l_insertar(tLista l, tPosicion p, tElemento e){
    tPosicion nuevo;
    if(l==NULL || p==NULL){
        exit(LST_POSICION_INVALIDA);
    }
    nuevo=(tPosicion) malloc(sizeof(struct celda));
    if(nuevo==NULL)
        exit(LST_ERROR_MEMORIA);
    nuevo->elemento=e;
    (nuevo->siguiente) = p->siguiente;
    (p->siguiente) = nuevo;
}
/**
 Elimina la celda P de L.
 El elemento almacenado en la posición P es eliminado mediante la función fEliminar.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
void l_eliminar(tLista l, tPosicion p, void(*fEliminar)(tElemento)){
    tPosicion pos_eliminar;
    if(p->siguiente==NULL) //si P es el fin
        exit(LST_POSICION_INVALIDA);
    pos_eliminar=p->siguiente;
    p->siguiente = pos_eliminar->siguiente;
    fEliminar(pos_eliminar->elemento);
    pos_eliminar->elemento=NULL;
    pos_eliminar->siguiente=NULL;
    free(pos_eliminar);
}

void l_destruir_recursivo(tPosicion p,void (*fEliminar)(tElemento)){
    if(p->siguiente!=NULL)
        l_destruir_recursivo(p->siguiente, fEliminar);

    fEliminar(p->elemento);
    p->elemento=NULL;
    p->siguiente=NULL;
    free(p);
}
/**
 Destruye la lista L, elimininando cada una de sus celdas.
 Los elementos almacenados en las celdas son eliminados mediante la función fEliminar.
**/
void l_destruir(tLista * l,void (*fEliminar)(tElemento)){
    tLista lista=*l;
    tPosicion pos=lista;
    if(pos->siguiente != NULL)
        l_destruir_recursivo(pos->siguiente,fEliminar);

    pos->siguiente=NULL;
    pos->elemento=NULL;
    free(pos);
    *l=NULL;
}
 /**
 Recupera y retorna el elemento en la posición P.
 Finaliza indicando LST_POSICION_INVALIDA si P es fin(L).
**/
tElemento l_recuperar(tLista l, tPosicion p){
    if(p==NULL || (p->siguiente)==NULL)
        exit(LST_POSICION_INVALIDA);
    return (p->siguiente)->elemento;
}
/**
 Recupera y retorna la primera posición de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_primera(tLista l){
    return l; //TA BN
}
/**
 Recupera y retorna la posición siguiente a P en L.
 Finaliza indicando LST_NO_EXISTE_SIGUIENTE si P es fin(L).
**/
tPosicion l_siguiente(tLista l, tPosicion p){
    if(p->siguiente==NULL)  //TA BN
        exit(LST_POSICION_INVALIDA);

    return (p->siguiente);
}
/**
 Recupera y retorna la posición anterior a P en L.
 Finaliza indicando LST_NO_EXISTE_ANTERIOR si P es primera(L).
**/
tPosicion l_anterior(tLista l, tPosicion p){
    tPosicion pos=l;
    if(p==l)
        exit(LST_NO_EXISTE_ANTERIOR);
    while(pos->siguiente!=p)
        pos = pos->siguiente;
    return pos;
}
/**
 Recupera y retorna la última posición de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_ultima(tLista l){
    tPosicion ultima, actual;
    ultima=actual=l;
    while(actual->siguiente!=NULL){
        ultima=actual;
        actual=actual->siguiente;
    }
    return ultima;
}
/**
 Recupera y retorna la posición fin de L.
 Si L es vacía, primera(L) = ultima(L) = fin(L).
**/
tPosicion l_fin(tLista l){
    tPosicion fin;
    fin=l;
    while(fin->siguiente!=NULL)
        fin=fin->siguiente;
    return fin;
}
/**
 Retorna la longitud actual de la lista.
**/

int l_longitud(tLista l){
    int cantidad=0;
    tPosicion cursor=l;
    while(cursor->siguiente!=NULL){
        cantidad++;
        cursor=cursor->siguiente;
    }
    return cantidad;
}
