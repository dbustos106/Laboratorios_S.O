#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "./Estructuras/HashTable.h"

void leerEncabezado(FILE* file){
    char* file_content = (char*) malloc(135*sizeof(char));
    if(file_content == NULL){
        printf("Error en la asignación de memoria del encabezado\n");
        exit(-1);
    }
    fscanf(file, "%[^\n]", file_content);
    free(file_content);
}

FILE* openFile(FILE* file, char* dir, char* modo){
    file=fopen(dir, modo);
    if(file==NULL){
        printf("Error leyendo archivo\n");
    }else{
        printf("Archivo leido con exito\n");
    }
    return file;
}

int main(){
    //Leer el archivo bogota-cadastral-2020
    FILE *file;
    file = openFile(file, "./Archivos/bogota-cadastral-2020-1-All-HourlyAggregate.dat", "rb");

    //Crear un archivo para almacenar la HashTable
    FILE *fileHashTable;
    fileHashTable = openFile(fileHashTable, "./Archivos/fileHashTable.dat", "w+");

    //Crear un archivo para almacenar las linkedList
    FILE *fileLinkedLists;
    fileLinkedLists = openFile(fileLinkedLists, "./Archivos/fileLinkedLists.dat", "w+");

    HashTable* hashTable = createHashTable();

    //Leer encabezado de la tabla
    leerEncabezado(file);     
    
    //Leer cada uno de los datos e insertarlos en la hashTable
    while(!feof(file)){
        Travel* travel = createTravel();
        bool lectura = readTravel(travel, file);
        if(lectura == false){
            break;
        }
        insertHash(fileHashTable, fileLinkedLists, hashTable, travel);
        free(travel);
    }

    fclose(file);
    fclose(fileHashTable);
    fclose(fileLinkedLists);
    
    //ELiminar la hash
    free(hashTable);

    return 0;
}