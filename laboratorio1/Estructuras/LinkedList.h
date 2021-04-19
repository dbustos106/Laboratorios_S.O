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

LinkedList createList(){
    LinkedList linkedList;
    linkedList.head = NULL;
    linkedList.tail = NULL; 
    linkedList.size = 0;
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
    return;
}

void eliminarLinkedList(LinkedList* linkedList){
    while(linkedList->head != NULL){
        Node* node = linkedList->head->next;
        eliminarNodo(linkedList->head);
        linkedList->head = node;
    }
}

#endif