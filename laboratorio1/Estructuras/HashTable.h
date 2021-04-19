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
    for(int i = 0; i < 1160; i++){
        hashTable->arreglo[i] = initList();
    }
    return hashTable;
}

int getHash (int id) {
    return id;
}

void insertHash(FILE* fileHashTable, FILE* fileLinkedLists, HashTable* hashTable, int key, Travel* travel){ 
    fseek(fileLinkedLists, 0, SEEK_END);
    int curLinkedList = ftell(fileLinkedLists);
    writeTravel(fileLinkedLists, key, travel, -1);

    if(hashTable->arreglo[key-1].headCur == -1){
        fseek(fileHashTable, 0, SEEK_END);
        int posHashTable = ftell(fileHashTable);
        createList(&hashTable->arreglo[key-1], posHashTable, curLinkedList, curLinkedList);

        fwrite(&key,sizeof(int),1,fileHashTable);
        fwrite(&curLinkedList,sizeof(int),1,fileHashTable);
        fwrite(&curLinkedList,sizeof(int),1,fileHashTable);
        
    }else{
        //Conseguir la posición de la lista enlazada
        int posHashTable = getPosHashTable(&hashTable->arreglo[key-1]);

        //Cambiar el nextCur
        fseek(fileLinkedLists, hashTable->arreglo[key-1].tailCur + 3*sizeof(int) + 5*sizeof(double), SEEK_SET);
        fwrite(&curLinkedList,sizeof(int),1,fileLinkedLists);
        
        //Cambiar la cola de la lista enlazada
        fseek(fileHashTable, posHashTable + 2*sizeof(int), SEEK_SET);
        fwrite(&curLinkedList,sizeof(int),1,fileHashTable);
        hashTable->arreglo[key-1].tailCur = curLinkedList;
    }
    
}

#endif