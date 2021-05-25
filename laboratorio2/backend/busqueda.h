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
    //Recuperar de fileHashTable el headCur que apunta a la cabeza de la lista enlazada
    fseek(fileHashTable, (sourceId-1)*2*sizeof(int) + sizeof(int), SEEK_SET);
    int r = fread(&readHeadCur,sizeof(int),1,fileHashTable);
    //Validar error
    if(r == 0){
        return -1;
    }

    Travel* travel = createTravel();

    do{
        //Leer el destino y la hora del registro leido actualmente
        fseek(fileLinkedList,readHeadCur+sizeof(int),SEEK_SET);
        fread(&travel->dstid,sizeof(int),1,fileLinkedList);
        fread(&travel->hod,sizeof(int),1,fileLinkedList);
        if(travel->dstid == dstId && travel->hod == hod){
            //Si los datos de destino y hora coinsiden, 
            //se obtiene el tiempo medio y se retorna el dato
            fread(&travel->mean_travel_time,sizeof(double),1,fileLinkedList);
            return travel->mean_travel_time;
        }else{
            //Si no, se obtiene el apuntador nextCur al siguiente registro de la lista enlazada
            fseek(fileLinkedList, sizeof(double),SEEK_CUR);
            fread(&readHeadCur,sizeof(int),1,fileLinkedList);
        }
    }while(readHeadCur!=-1);
    
    free(travel);
    printf("NA");
    return -1.0;
}

#endif