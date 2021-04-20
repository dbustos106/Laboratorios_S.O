#ifndef HashTable_h
#define HashTable_h

#include<stdio.h>
#include<stdlib.h>
#include "LinkedList.h"
#include "Travel.h"

typedef struct HashTable{
    struct LinkedList arreglo[1160];
}HashTable;

HashTable* createHashTable(){
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    if(hashTable == NULL){
        printf("Error en la asignación de memoria del hashTable\n");
        exit(-1);
    }
    for(int i = 0; i < 1160; i++){
        hashTable->arreglo[i] = initList();
    }
    return hashTable;
}

void insertHash(FILE* fileHashTable, FILE* fileLinkedLists, HashTable* hashTable, Travel* travel){ 
    fseek(fileLinkedLists, 0, SEEK_END);
    int curLinkedList = ftell(fileLinkedLists);
    writeTravel(fileLinkedLists, travel, -1);

    if(hashTable->arreglo[travel->sourceid-1].headCur == -1){
        fseek(fileHashTable, 2*sizeof(int)*(travel->sourceid-1), SEEK_SET);

        createList(&hashTable->arreglo[travel->sourceid-1], curLinkedList, curLinkedList);

        fwrite(&travel->sourceid,sizeof(int),1,fileHashTable);
        fwrite(&curLinkedList,sizeof(int),1,fileHashTable);
        
    }else{
        //Conseguir la posición de la lista enlazada
        int posHashTable = 3*sizeof(int)*(travel->sourceid-1);

        //Cambiar el nextCur
        fseek(fileLinkedLists, hashTable->arreglo[travel->sourceid-1].tailCur + 3*sizeof(int) + sizeof(double), SEEK_SET);
        fwrite(&curLinkedList,sizeof(int),1,fileLinkedLists);
        
        //Cambiar la cola de la lista enlazada
        hashTable->arreglo[travel->sourceid-1].tailCur = curLinkedList;
    }
}

#endif