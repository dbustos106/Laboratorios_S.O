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

void insertHash(HashTable * hashTable, int key, Travel* travel){
    insertTail(&hashTable->arreglo[key-1], key, travel);
}

double hodSearch(HashTable* hashTable, int sourceId, int dst){
    return NodeSearch(&hashTable->arreglo[sourceId-1], sourceId, dst);
}

void eliminarHashTable(HashTable* hashTable){
    for(int i = 0; i < 1160; i++){
        eliminarLinkedList(&hashTable->arreglo[i]);
    }
    free(hashTable);
}

#endif