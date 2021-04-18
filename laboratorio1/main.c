#include<stdio.h>
#include<stdlib.h>
#include "./Estructuras/HashTable.h"

void leerEncabezado(FILE* file){
    char* file_content = (char*) malloc(135*sizeof(char));
    int leidos = fscanf(file, "%[^\n]", file_content);
    free(file_content);
}

void read(Travel* travel, FILE* file){
	//long unsigned int curPos=(7*sizeof(double) + 6*sizeof(char))*position + 135*sizeof(char);
 
    char* comas = (char*) malloc(sizeof(char));

    fscanf(file, "%d", &travel->sourceid);
    fscanf(file, "%c", comas);
    fscanf(file, "%d", &travel->dstid);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &travel->hod);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &travel->mean_travel_time);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &travel->standard_deviation_travel_time);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &travel->geometric_mean_travel_time);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &travel->geometric_standard_deviation_travel_time);
    fscanf(file, "%c", comas);
    
    free(comas);
}


int main(){
    FILE *file;
    printf("Inicio programa\n");
    file=fopen("bogota-cadastral-2020-1-All-HourlyAggregate.txt", "rb");
    if(file==NULL){
        printf("Error leyendo archivo \n");
    }else{
        printf("Exito\n");
    }

    HashTable* hashTable = createHashTable();

    leerEncabezado(file);
    int i=0;
    while(!feof(file)){
        Travel* travel = createTravel();
        read(travel, file);
        insertHash(hashTable, travel->sourceid, travel);
        free(travel);
        i++;
    }

    double media = hodSearch(hashTable, 1, 75);
    printf("Media: %f", media);


    printf("\nHola Mundo1\n");
    eliminarHashTable(hashTable);
    printf("\nHola Mundo2\n");


    fclose(file);
    return 0;
}