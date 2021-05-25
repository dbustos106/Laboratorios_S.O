#ifndef HashTable_h
#define HashTable_h

#include<stdio.h>
#include<stdlib.h>
#include "LinkedList.h"
#include "Travel.h"

typedef struct HashTable{
    struct LinkedList arreglo[1160];
}HashTable;

//Reservar un espacio de memoria para almacenar la hashTable.
HashTable* createHashTable(){
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    if(hashTable == NULL){
        printf("Error en la asignación de memoria del hashTable\n");
        exit(-1);
    }
    for(int i = 0; i < 1160; i++){
        //Inicializar los valores de head y tail en -1.
        hashTable->arreglo[i] = initList();
    }
    return hashTable;
}

void insertHash(FILE* fileHashTable, FILE* fileLinkedLists, HashTable* hashTable, Travel* travel){ 
    fseek(fileLinkedLists, 0, SEEK_END);
    int curLinkedList = ftell(fileLinkedLists);

    //Escribir sobre fileLinkedLists los datos del siguiente travel. 
    writeTravel(fileLinkedLists, travel, -1);

    if(hashTable->arreglo[travel->sourceid-1].headCur == -1){
        fseek(fileHashTable, 2*sizeof(int)*(travel->sourceid-1), SEEK_SET);

        //Cambiar los apuntadores head y tail de la lista
        createList(&hashTable->arreglo[travel->sourceid-1], curLinkedList, curLinkedList);

        //Escribir sobre fileHashTable los datos de la lista. Key y head.
        fwrite(&travel->sourceid,sizeof(int),1,fileHashTable);
        fwrite(&curLinkedList,sizeof(int),1,fileHashTable);
        
    }else{
        //Cambiar el nextCur
        fseek(fileLinkedLists, hashTable->arreglo[travel->sourceid-1].tailCur + 3*sizeof(int) + sizeof(double), SEEK_SET);
        fwrite(&curLinkedList,sizeof(int),1,fileLinkedLists);
        
        //Cambiar la cola de la lista enlazada
        hashTable->arreglo[travel->sourceid-1].tailCur = curLinkedList;
    }
}

#endif