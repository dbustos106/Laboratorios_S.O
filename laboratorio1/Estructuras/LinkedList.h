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

LinkedList* createList(){
    LinkedList *linkedList = (LinkedList*)malloc(sizeof(LinkedList));
    linkedList->head = NULL;
    linkedList->tail = NULL; 
    linkedList->size = 0;
    return linkedList;
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

double NodeSearch(LinkedList* list, int sourceId, int dst){
    Node* node = list->head;
    for(int i = 0; i < list->size; i++){
        if(node->travel.dstid == dst){
            return node->travel.hod;
        }
        node = node->next;
    }
    return -1;    
}

void eliminarLinkedList(LinkedList* linkedList){
    for(int i = 0; i < linkedList->size; i++){
        Node* node = linkedList->head->next;
        eliminarNodo(linkedList->head);
        linkedList->head = node;
    }
}

#endif