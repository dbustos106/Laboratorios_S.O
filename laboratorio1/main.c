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

    //Crear una HashTable
    HashTable* hashTable = createHashTable();

    //Crear un archivo para almacenar la HashTable
    FILE *fileHashTable;
    fileHashTable = openFile(fileHashTable, "fileHashTable.txt", "w+");

    //Crear un archivo para almacenar las linkedList
    FILE *fileLinkedLists;
    fileLinkedLists = openFile(fileLinkedLists, "fileLinkedLists.txt", "w+");

    //Leer encabezado de la tabla
    leerEncabezado(file);
    //Leer cada uno de los datos e insertarlos en la hashTable
    int i = 0;
    while(!feof(file)){
        Travel* travel = createTravel();
        bool lectura = read(travel, file);
        if(lectura == false){
            break;
        }
        insertHash(fileHashTable, fileLinkedLists, hashTable, travel->sourceid, travel);
        //if(travel->sourceid == 951){
        //    printf("%d ", i);
        //}
        free(travel);
        if(i==2021){
            break;
        }
        i++;
    }
    fclose(file);

    fseek(fileHashTable, 0, SEEK_SET);
    int k = 0;
    printf("HASHTABLE\n");
    while(!feof(fileHashTable)){
        int cadena;
        int cadena1;
        int cadena2;
        fread(&cadena, sizeof(int),1,fileHashTable);
        fread(&cadena1, sizeof(int),1,fileHashTable);
        fread(&cadena2, sizeof(int),1,fileHashTable);
        if(k < 4){
            printf("%d,", cadena);
            printf("%d,", cadena1);
            printf("%d\n", cadena2);
        }
        k++;
    }


    printf("\n\n");
    fseek(fileLinkedLists, 0, SEEK_SET);
    int j = 0;
    while(!feof(fileLinkedLists)){
        int cadena;
        int cadena1;
        int cadena2;
        double cadena3;
        double cadena4;
        double cadena5;
        double cadena6;
        double cadena7;
        int cadena8;
        fread(&cadena, sizeof(int),1,fileLinkedLists);
        fread(&cadena1, sizeof(int),1,fileLinkedLists);
        fread(&cadena2, sizeof(int),1,fileLinkedLists);
        fread(&cadena3, sizeof(double),1,fileLinkedLists);
        fread(&cadena4, sizeof(double),1,fileLinkedLists);
        fread(&cadena5, sizeof(double),1,fileLinkedLists);
        fread(&cadena6, sizeof(double),1,fileLinkedLists);
        fread(&cadena7, sizeof(double),1,fileLinkedLists);
        fread(&cadena8, sizeof(int),1,fileLinkedLists);
        //if((j > 0 && j < 7)|| (j > 382 && j < 387) || (j > 573 && j < 577)|| (j > 643 && j < 647)){
        if((j > 1 && j < 7)|| (j > 696 && j < 703) || (j > 2004 && j < 2010)){
            printf("%d,", cadena);
            printf("%d,", cadena1);
            printf("%d,", cadena2);
            printf("%f,", cadena3);
            printf("%f,", cadena4);
            printf("%f,", cadena5);
            printf("%f,", cadena6);
            printf("%f,", cadena7);
            printf("%d\n", cadena8);
        }
        j++;
    }



    fclose(fileHashTable);
    fclose(fileLinkedLists);
    
    //ELiminar la hash
    free(hashTable);

    return 0;
}