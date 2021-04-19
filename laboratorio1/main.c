#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "./Estructuras/HashTable.h"

void leerEncabezado(FILE* file){
    char* file_content = (char*) malloc(135*sizeof(char));
    int leidos = fscanf(file, "%[^\n]", file_content);
    free(file_content);
}

bool read(Travel* travel, FILE* file){
    bool lectura = true;
    char* comas = (char*) malloc(sizeof(char));

    if(fscanf(file, "%d%c", &travel->sourceid, comas) == EOF){
        lectura = false;
    }

    if(fscanf(file, "%d%c", &travel->dstid, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->hod, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->mean_travel_time, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->standard_deviation_travel_time, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->geometric_mean_travel_time, comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf%c", &travel->geometric_standard_deviation_travel_time, comas) == EOF){
        lectura = false;
    }

    free(comas);
    return lectura;
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
    file = openFile(file, "bogota-cadastral-2020-1-All-HourlyAggregate.dat", "rb");

    //Crear un archivo para almacenar la HashTable
    FILE *fileHashTable;
    fileHashTable = openFile(fileHashTable, "fileHashTable.txt", "w+");

    //Crear un archivo para almacenar las linkedList
    FILE *fileLinkedLists;
    fileLinkedLists = openFile(fileLinkedLists, "fileLinkedLists.txt", "w+");

    HashTable* hashTable = createHashTable();

    //Leer encabezado de la tabla
    leerEncabezado(file);     
    
    //Leer cada uno de los datos e insertarlos en la hashTable
    while(!feof(file)){
        Travel* travel = createTravel();
        bool lectura = read(travel, file);
        if(lectura == false){
            break;
        }
        insertHash(fileHashTable, fileLinkedLists, hashTable, travel->sourceid, travel);
        free(travel);
    }
    fclose(file);
    fclose(fileHashTable);
    fclose(fileLinkedLists);
    
    //ELiminar la hash
    free(hashTable);

    return 0;
}