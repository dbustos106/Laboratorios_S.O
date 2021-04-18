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
    return hashTable;
}

int getHash (int id) {
    return id;
}

void insertHash(HashTable * hashTable, int key, Travel* travel){
    insertTail(&hashTable->arreglo[key], key, travel);
}

#endif