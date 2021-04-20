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
    printf("%d, %d, %d", sourceId, dstId, hod);
    FILE* fileHashTable;
    fileHashTable = openFile(fileHashTable, "./Archivos/fileHashTable.dat", "rb");
    FILE* fileLinkedList;
    fileLinkedList = openFile(fileLinkedList, "./Archivos/fileLinkedLists.dat", "rb");

    int i = 0;
    int readHeadCur;
    int readTailCur;
    while(!feof(fileHashTable)){
        int readSourceId;
        fseek(fileHashTable, 3*i*sizeof(int) ,SEEK_SET);
        fread(&readSourceId,sizeof(int),1,fileHashTable);
        if(sourceId == readSourceId){
            fread(&readHeadCur,sizeof(int),1,fileHashTable);
            fread(&readTailCur,sizeof(int),1,fileHashTable);
            break;
        }
        i++;
    }
    printf("%d, %d \n",readHeadCur,readTailCur);

    do{
        fseek(fileLinkedList,readHeadCur+2*sizeof(int),SEEK_CUR);
        int readDstId;
        fread(&readDstId,sizeof(int),1,fileLinkedList);
        int readHod;
        fread(&readHod,sizeof(int),1,fileLinkedList);
        if(readDstId == dstId && readHod == hod){
            double readMean_time;
            fread(&readMean_time,sizeof(int),1,fileLinkedList);
            return readMean_time;
        }else{
            fseek(fileLinkedList,5*sizeof(double),SEEK_CUR);
            fread(&readHeadCur,sizeof(int),1,fileHashTable);
        }
    }while(readHeadCur!=-1);
    printf("NA");
    return 0.0;
}
#endif