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

    if(fscanf(file, "%d", &travel->sourceid) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%d", &travel->dstid) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf", &travel->hod) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf", &travel->mean_travel_time) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf", &travel->standard_deviation_travel_time) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf", &travel->geometric_mean_travel_time) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%lf", &travel->geometric_standard_deviation_travel_time) == EOF){
        lectura = false;
    }
    if(fscanf(file, "%c", comas) == EOF){
        lectura = false;
    }
    return lectura;
    free(comas);
}


int main(){
    FILE *file;
    printf("Inicio programa\n");
    file=fopen("bogota-cadastral-2020-1-All-HourlyAggregate.txt", "rb");
    if(file==NULL){
        printf("Error leyendo archivo \n");
    }else{
        printf("Archivo leido con exito\n");
    }

    HashTable* hashTable = createHashTable();

    leerEncabezado(file);
    while(!feof(file)){
        Travel* travel = createTravel();
        bool lectura = read(travel, file);
        if(lectura == false){
            break;
        }
        insertHash(hashTable, travel->sourceid, travel);
        free(travel);
    }
    fclose(file);

    //double media = hodSearch(hashTable, 1, 75);
    //printf("Media: %f", media);
    
    FILE *fileHashTable;
    fileHashTable = fopen("fileHashTable.txt", "wb");
    if(fileHashTable==NULL){
        printf("Error leyendo archivo \n");
    }else{
        printf("Archivo leido con exito\n");
    }
    FILE *fileLinkedLists;
    fileLinkedLists = fopen("fileLinkedList.txt", "wb");
    if(fileLinkedLists==NULL){
        printf("Error leyendo archivo \n");
    }else{
        printf("Archivo leido con exito\n");
    }
    
    escribirHashTable(hashTable, fileHashTable, fileLinkedLists);

    fclose(fileHashTable);
    fclose(fileLinkedLists);
    
    eliminarHashTable(hashTable);
    return 0;
}