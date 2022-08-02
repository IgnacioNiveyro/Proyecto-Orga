#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "mapeo.h"

#define FINALIZACIONEXITOSA 0
#define ARCHIVOINVALIDO -1
#define OPCIONERRONEA -2

tMapeo mapeo;

/**
    Elimina una clave.
**/
void fEliminarC(tClave clave){

    free(clave);
}

/**
    Elimina un valor.
**/
void fEliminarV(tValor valor){

    free(valor);
}

/**
    Compara dos cadenas.
    Devuelve: 0 si la cadenas son iguales, un numero positivo si la primer cadena es mayor que la segunda o un número negativo en el caso contrario.
**/

int fComparacion(void* s1, void* s2)
{
    char *string1 = s1;
    char *string2 = s2;
    int toReturn = strcmp(string1, string2);
    return toReturn;
}

/**
    Retorna la funcion hash de una clave recibida.
**/
int fHash(void* s)
{
    char* string = s;
    int hash = 0;
    int i;

    for (i = 0; i < strlen(string); i++){

        hash = 31 * hash + string[i];
    }

    return abs(hash);
}

/**
    Muestra el menú.
**/
void mostrarMenu(){
    printf("Test Main:\n\n");
    printf("1. Cantidad de Apariciones.\n");
    printf("2. Salir.\n\n");
}

/*void mapeoaarchivo(char * filename){

    FILE * fp = fopen(filename, "r");
    if (!fp){
        printf("No se pudo abrir el archivo '%s'. Codigo de error: %i",filename,ARCHIVOINVALIDO);
        exit(ARCHIVOINVALIDO);
    }

   // char linea[100]="\0";

    char delimitadores[] = ", ¿?¡!\n\r\0";
    char *palabras_renglon, *palabra, *str= (char *)malloc(100);
    tValor cant_palabras;
    crear_mapeo(&mapeo, 10, &fHash, &fComparacion);

    while (str!=NULL){

        str=fgets(linea,100,fp); //obtengo el "n" renglon del archivo
        palabras_renglon = strtok(str,delimitadores); //aca tengo todas las palabras del renglon

        while(palabras_renglon!=NULL){

            palabra = strdup(palabras_renglon);//guardo la primer palabra del renglon
            cant_palabras =  m_recuperar(mapeo,palabra);
            int * valor;
            valor = malloc(sizeof int);
            (*valor) = 0;
            (*valor)=(*valor)+cant_palabras;
            m_insertar(mapeo, palabra, (*valor));
            palabras_renglon = strtok(NULL, delimitadores);
        }
    }
    fclose(fp);
}*/

void cargar_archivo(char *filename){

    FILE *fp = fopen(filename, "r");

    if (!fp){
        perror("No se pudo abrir el archivo.");
    }

    crear_mapeo(&mapeo, 10, &fHash, &fComparacion);

    tValor cant_palabras;
   	char * str;
    int * valor;

    while (!feof(fp)){

      	str = malloc(sizeof (char)*250);
        valor  = malloc(sizeof (int));
        fscanf(fp,"%s",str);

        cant_palabras =  m_recuperar(mapeo,str);
        (*valor) = 1;

        if(cant_palabras != NULL){ // si la palabra ya estaba en el mapeo

            (*valor) = (*valor)+(*(int *)cant_palabras);
            m_insertar(mapeo, str, valor);
            free(cant_palabras);
            free (str);
        }
        else{ //si la pslsbra no estaba en el mapeo, la inserto

            m_insertar(mapeo, str, valor);
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[]){

    mapeo = NULL;

  	char * palabra;
    int opcion;

    if(argc == 1){
            // el archivo es nulo, vacío. Porque argc representa lo que me pasaron por consola, si tengo argc = 1, solo es el programa. No hay archivo.
       printf("ERROR.No ha ingresado ningun nombre de archivo. Codigo de error: %i \n",ARCHIVOINVALIDO);
       exit(ARCHIVOINVALIDO);
    }
    else if (argc > 2){

       printf("ERROR. No respeta el formato solicitado. Codigo de error: %i \n",ARCHIVOINVALIDO); // en realidad sería otro pero no se puede tener mas #define
       exit(ARCHIVOINVALIDO);
    }
    else
        cargar_archivo(argv[1]);


    palabra = malloc(sizeof (char)*250);
    while(0 == 0){

        mostrarMenu();
        scanf("%d", &opcion);

        if (opcion!=1 && opcion!=2){

            printf("La opcion elegida es erronea. Codigo de error: %i \n",OPCIONERRONEA);
            exit(OPCIONERRONEA);
        }
        else {
            switch(opcion){

            case 1:


                printf("Ingrese la palabra: ");

                scanf("%s", palabra);
                printf("La palabra que pusiste es '%s' \n",palabra);
                tValor cantidad_apariciones_puntero = m_recuperar(mapeo,palabra);

                if( cantidad_apariciones_puntero == NULL){// si la palabra no estaba en el mapeo

                    printf("La palabra '%s' no aparece en el archivo.\n", palabra);
                }
                else{

                    printf("La cantidad de veces que aparece '%s' es: %d \n", palabra, (*(int*)cantidad_apariciones_puntero));
                }

                free(cantidad_apariciones_puntero);
                fflush(stdin);

                break;
            case 2:

                m_destruir(&mapeo, &fEliminarC, &fEliminarV);
                printf("Finalizacion exitosa! Codigo: %i \n",FINALIZACIONEXITOSA);
                exit(FINALIZACIONEXITOSA);
            }
        }
	}

    free(palabra);

    return 0;
}
