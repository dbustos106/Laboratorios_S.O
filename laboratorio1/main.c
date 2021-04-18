#include<stdio.h>
#include<stdlib.h>
#include "./Estructuras/HashTable.h"

void leerEncabezado(FILE* file){
    char* file_content = malloc(135*sizeof(char));
    int leidos = fscanf(file, "%[^\n]", file_content);
    free(file_content);
}

void read(FILE* file){
	//long unsigned int curPos=(7*sizeof(double) + 6*sizeof(char))*position + 135*sizeof(char);

    Travel* file_content2 = createTravel();    
    char* comas = malloc(sizeof(char));

    fscanf(file, "%lf", &file_content2->sourceid);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &file_content2->dstid);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &file_content2->hod);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &file_content2->mean_travel_time);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &file_content2->standard_deviation_travel_time);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &file_content2->geometric_mean_travel_time);
    fscanf(file, "%c", comas);
    fscanf(file, "%lf", &file_content2->geometric_standard_deviation_travel_time);
    fscanf(file, "%c", comas);
    
    free(file_content2);
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

    leerEncabezado(file);
    while(!feof(file)){
        read(file);
    }

    printf("Hola Mundo");

    fclose(file);
    return 0;
}