#ifndef HashTable_h
#define HashTable_h

#include<stdio.h>
#include<stdlib.h>
#include "LinkedList.h"

typedef struct HashTable{
    struct LinkedList arreglo[1160];
}HashTable;

HashTable* createHashTable(){
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    for(int i = 0; i < 1160; i++){
        hashTable->arreglo[i] = createList();
    }
    return hashTable;
}

int getHash (int id) {
    return id;
}

void insertHash(FILE* fileHashTable, FILE* fileLinkedLists, HashTable* hashTable, int key, Travel* travel){ 
    fseek(fileLinkedLists, 0, SEEK_END);
    int cur = ftell(fileLinkedLists);
    int next = -1;
    fwrite(&key, sizeof(int),1,fileLinkedLists);
    fwrite(&travel->sourceid,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->dstid,sizeof(int),1,fileLinkedLists);
    fwrite(&travel->hod,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->mean_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->standard_deviation_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->geometric_mean_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&travel->geometric_standard_deviation_travel_time,sizeof(double),1,fileLinkedLists);
    fwrite(&next,sizeof(int),1,fileLinkedLists);
    if(hashTable->arreglo[key-1].head == NULL){
        fseek(fileHashTable, 0, SEEK_END);
        int curHash = ftell(fileHashTable);

        hashTable->arreglo[key-1].size = curHash;
        
        fwrite(&key,sizeof(int),1,fileHashTable);
        fwrite(&cur,sizeof(int),1,fileHashTable);
        fwrite(&cur,sizeof(int),1,fileHashTable);
    }else{
        int curH = hashTable->arreglo[key-1].size;
        fseek(fileHashTable, curH + 2*sizeof(int), SEEK_SET);
        int tail;
        fread(&tail,sizeof(int),1,fileHashTable);

        fseek(fileLinkedLists, tail + 3*sizeof(int) + 5*sizeof(double), SEEK_SET);
        fwrite(&cur,sizeof(int),1,fileLinkedLists);
        
        fseek(fileHashTable, curH + 2*sizeof(int), SEEK_SET);
        fwrite(&cur,sizeof(int),1,fileHashTable);
    }
    insertTail(&hashTable->arreglo[key-1], key, travel);
}

void eliminarHashTable(HashTable* hashTable){
    for(int i = 0; i < 1160; i++){
        eliminarLinkedList(&hashTable->arreglo[i]);
    }
    free(hashTable);
}

#endif