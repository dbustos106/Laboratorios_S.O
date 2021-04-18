#ifndef LinkedList_h
#define LinkedList_h

#include<stdio.h>
#include<stdlib.h>
#include "Node.h"

typedef struct LinkedList{
    struct Node *head;
    struct Node *tail;
    int size;
}LinkedList;

void createList(){
    LinkedList *linkedList = malloc(sizeof(LinkedList));
    linkedList->head = NULL;
    linkedList->tail = NULL; 
    linkedList->size = 0;
}

void insertTail(LinkedList *list, int key, Travel* travel) {
    Node* node = createNode(key, travel);

    if (list->head == NULL) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    list->size = list->size + 1;
    return;
}

#endif