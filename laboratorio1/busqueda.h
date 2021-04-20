#ifndef busqueda_h
#define busqueda_h

#include<stdio.h>
#include<stdlib.h>

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

    int i = 0;
    int readHeadCur;
    int readTailCur;
    fseek(fileHashTable, (sourceId-1)*3*sizeof(int) + sizeof(int), SEEK_SET);
    fread(&readHeadCur,sizeof(int),1,fileHashTable);
    fread(&readTailCur,sizeof(int),1,fileHashTable);
    int j = 0;
    do{
        fseek(fileLinkedList,readHeadCur+sizeof(int),SEEK_SET);
        int readDstId;
        fread(&readDstId,sizeof(int),1,fileLinkedList);
        int readHod;
        fread(&readHod,sizeof(int),1,fileLinkedList);
        if(readDstId == dstId && readHod == hod){
            double readMean_time;
            fread(&readMean_time,sizeof(double),1,fileLinkedList);
            return readMean_time;
        }else{
            fseek(fileLinkedList, 4*sizeof(double),SEEK_CUR);
            fread(&readHeadCur,sizeof(int),1,fileLinkedList);
        }
    }while(readHeadCur!=-1);
    printf("NA");
    return -1.0;
}
#endif