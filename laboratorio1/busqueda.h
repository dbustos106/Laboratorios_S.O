#ifndef busqueda_h
#define busqueda_h

#include<stdio.h>
#include<stdlib.h>
#include"./Estructuras/Travel.h"

FILE* openFile(FILE* file, char* dir, char* modo){
    file=fopen(dir, modo);
    if(file==NULL){
        printf("Error leyendo archivo\n");
    }else{
        printf("Archivo leido con exito\n");
    }
    return file;
}

double busqueda(int sourceId, int dstId, int hod){
    FILE* fileHashTable;
    fileHashTable = openFile(fileHashTable, "./Archivos/fileHashTable.dat", "rb");
    FILE* fileLinkedList;
    fileLinkedList = openFile(fileLinkedList, "./Archivos/fileLinkedLists.dat", "rb");

    int readHeadCur;
    fseek(fileHashTable, (sourceId-1)*2*sizeof(int) + sizeof(int), SEEK_SET);
    int r = fread(&readHeadCur,sizeof(int),1,fileHashTable);
    if(r == 0){
        return -1;
    }

    Travel* travel = createTravel();

    do{
        fseek(fileLinkedList,readHeadCur+sizeof(int),SEEK_SET);
        fread(&travel->dstid,sizeof(int),1,fileLinkedList);
        fread(&travel->hod,sizeof(int),1,fileLinkedList);
        if(travel->dstid == dstId && travel->hod == hod){
            fread(&travel->mean_travel_time,sizeof(double),1,fileLinkedList);
            return travel->mean_travel_time;
        }else{
            fseek(fileLinkedList, sizeof(double),SEEK_CUR);
            fread(&readHeadCur,sizeof(int),1,fileLinkedList);
        }
    }while(readHeadCur!=-1);
    
    free(travel);
    printf("NA");
    return -1.0;
}

#endif