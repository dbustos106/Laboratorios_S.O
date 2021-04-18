#ifndef Node_h
#define Node_h

#include<stdio.h>
#include<stdlib.h>
#include"Travel.h"

typedef struct Node{
    int key;
    struct Travel travel;  
    struct Node* next;
}Node;

Node* createNode(int key, Travel *travel){
    Node* node = (Node*) malloc(sizeof(Node));
    node->key = key;
    node->next = NULL; 
    node->travel = *travel;
    return node;
}

void eliminarNodo(Node* node){
    free(node);
}

#endif